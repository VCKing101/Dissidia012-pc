// Online play: delay-based lockstep over UDP (d012_net.hpp).
//
// Packets (little endian) start "D0NL", u8 kind, then a 24-bit link tag (0 for
// the link from start-up; a battle's own for a battle link), so a packet left
// over from an earlier battle is never taken for this one:
// - HELLO (1), player 2 to player 1: u32 version.
// - WELCOME (2), player 1's answer: u32 version, u32 input delay, u64 the PSP
//   clock's epoch, u8 length and the save answers.
// - INPUT (3): u64 how many of the other copy's frames this copy holds (all
//   frames below it), u64 the first frame carried, u16 their count, u16
//   spare, u64 a checked read and u64 its hash (read ~0 for none), u32 this
//   copy's clock in ms and u32 the other copy's clock from its latest INPUT
//   plus how long this copy has held it (~0 for none: the round trip is the
//   sender's clock less that echo), then 16 bytes a frame: u8 dialog, u8 save,
//   u8 connected, u8 lx, ly, rx, ry, spare, u32 buttons, u32 extra.
// - BYE (4): this copy has left (a battle link closed by a quit); nothing more
//   will come.
// Each INPUT carries every frame the other copy has not confirmed (up to 64),
// so a lost packet costs only the wait for the next one. The first `delay`
// frames carry nobody's input on either copy.
//
// The battle link's address and the input delay can also come from
// settings.ini (online.battle_link, online.input_delay), so that a copy needs
// no launch options.
//
// D012_NET_SIM=DELAY_MS[,JITTER_MS[,LOSS_PERCENT]] is a test switch: every
// packet this copy sends is held DELAY_MS give or take JITTER_MS (so packets
// may arrive out of order), or dropped LOSS_PERCENT times in a hundred, as an
// internet path would. Two copies on one PC each with that setting are a link
// with twice DELAY_MS of round trip.
//
// MIT licence; see port/README.md.

#include "d012_net.hpp"

#include "install/user_data.hpp"

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <iostream>
#include <map>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace d012::net {
namespace {

#if defined(_WIN32)
using Socket = SOCKET;
using AddressSize = int;
constexpr Socket kNoSocket = INVALID_SOCKET;
void close_socket(Socket s) { closesocket(s); }
#else
using Socket = int;
using AddressSize = socklen_t;
constexpr Socket kNoSocket = -1;
void close_socket(Socket s) { close(s); }
#endif

using Clock = std::chrono::steady_clock;

constexpr char kMagic[4] = {'D', '0', 'N', 'L'};
constexpr std::uint8_t kHello = 1u;
constexpr std::uint8_t kWelcome = 2u;
constexpr std::uint8_t kInput = 3u;
constexpr std::uint8_t kBye = 4u;
constexpr std::uint32_t kVersion = 3u;
constexpr std::size_t kHead = 8u;
constexpr std::size_t kInputHead = kHead + 48u;
constexpr std::size_t kLocalSize = 16u;
constexpr std::uint64_t kMaxCarried = 64u;
constexpr std::uint64_t kNoRead = ~0ull;
constexpr std::uint32_t kTagMask = 0xFFFFFFu;
constexpr std::uint32_t kNoTime = ~0u;
constexpr auto kResend = std::chrono::milliseconds(20);
constexpr auto kHelloEvery = std::chrono::milliseconds(250);
constexpr auto kLost = std::chrono::seconds(10);
constexpr auto kLongWait = std::chrono::milliseconds(100);
constexpr auto kLastFrames = std::chrono::milliseconds(1000);
constexpr std::uint64_t kReportEvery = 900u;  // frames between reports of the waits

struct Link {
    const char *name = "net";  // the log's prefix
    const char *unit = "read"; // what its frames are, for the log
    bool active = false;
    bool player1 = false;
    bool peer_known = false;
    bool welcomed = false;
    bool lost = false;
    bool left = false;  // the other copy said BYE
    Clock::time_point left_at{};
    bool apart = false;
    std::uint32_t tag = 0;
    std::uint32_t delay = 3u;
    std::uint64_t epoch = 0;
    std::string answers;
    Socket socket = kNoSocket;
    sockaddr_in peer{};
    std::map<std::uint64_t, Local> mine;    // by frame, from the oldest the other copy may still need
    std::map<std::uint64_t, Local> theirs;  // by frame, received and not yet used
    std::uint64_t their_count = 0;          // the other copy's frames held: all below this one
    std::uint64_t confirmed = 0;            // this copy's frames the other copy holds
    std::map<std::uint64_t, std::uint64_t> my_hashes;
    std::map<std::uint64_t, std::uint64_t> their_hashes;
    std::uint64_t hash_read = kNoRead;  // the latest checked frame, sent with every INPUT
    std::uint64_t hash = 0;
    std::uint64_t checks = 0;
    Clock::time_point heard{};
    Clock::time_point sent{};
    // The waits for the other copy since the last report.
    std::uint64_t report_from = 0;
    Clock::time_point report_began{};
    // The other copy's clock from its latest INPUT, and when it came.
    std::uint32_t their_time = kNoTime;
    Clock::time_point their_time_at{};
    // Round trips since the last report, in ms.
    std::uint64_t trips = 0;
    std::uint64_t trip_total = 0;
    std::uint32_t trip_least = kNoTime;
    std::uint32_t trip_most = 0;
    std::uint64_t waits = 0;
    Clock::duration waited{};
    Clock::duration longest{};
};

// The link from start-up (D012_NET): every pad read from boot.
Link &link() {
    static Link l;
    return l;
}

// A Communication Mode battle's link (D012_NET_BATTLE): the battle's frames.
Link &battle_link() {
    static Link l;
    return l;
}

Local idle() {
    Local l;
    l.pad.connected = true;
    return l;
}

void put16(std::vector<std::uint8_t> &out, std::size_t at, std::uint16_t v) { std::memcpy(out.data() + at, &v, 2u); }
void put32(std::vector<std::uint8_t> &out, std::size_t at, std::uint32_t v) { std::memcpy(out.data() + at, &v, 4u); }
void put64(std::vector<std::uint8_t> &out, std::size_t at, std::uint64_t v) { std::memcpy(out.data() + at, &v, 8u); }
template <typename T> T get(const std::uint8_t *in) {
    T v;
    std::memcpy(&v, in, sizeof v);
    return v;
}

std::uint32_t clock_ms() {
    static const Clock::time_point start = Clock::now();
    return static_cast<std::uint32_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count());
}

std::vector<std::uint8_t> packet(const Link &l, std::uint8_t kind, std::size_t size) {
    std::vector<std::uint8_t> out(size, 0u);
    std::memcpy(out.data(), kMagic, 4u);
    out[4] = kind;
    out[5] = static_cast<std::uint8_t>(l.tag & 0xFFu);
    out[6] = static_cast<std::uint8_t>((l.tag >> 8u) & 0xFFu);
    out[7] = static_cast<std::uint8_t>((l.tag >> 16u) & 0xFFu);
    return out;
}

std::uint32_t tag_of(const std::uint8_t *data) {
    return static_cast<std::uint32_t>(data[5]) | (static_cast<std::uint32_t>(data[6]) << 8u) |
           (static_cast<std::uint32_t>(data[7]) << 16u);
}

void send_now(Socket socket, const sockaddr_in &to, const std::vector<std::uint8_t> &out) {
    sendto(socket, reinterpret_cast<const char *>(out.data()), static_cast<int>(out.size()), 0,
           reinterpret_cast<const sockaddr *>(&to), sizeof to);
}

// D012_NET_SIM: packets held on a thread of their own until they are due.
class Simulated {
public:
    // Never destroyed: its thread runs until the process ends.
    static Simulated &get() {
        static Simulated *s = new Simulated;
        return *s;
    }
    bool on() const { return on_; }
    void send(Socket socket, const sockaddr_in &to, const std::vector<std::uint8_t> &out) {
        std::lock_guard lock(mutex_);
        if (std::uniform_real_distribution<double>(0.0, 100.0)(random_) < loss_) return;
        const double jitter =
            jitter_ms_ > 0.0 ? std::uniform_real_distribution<double>(-jitter_ms_, jitter_ms_)(random_) : 0.0;
        const auto due = Clock::now() + std::chrono::microseconds(
                                            static_cast<long long>(std::max(0.0, delay_ms_ + jitter) * 1000.0));
        held_.emplace(due, Held{socket, to, out});
        wake_.notify_one();
    }

private:
    struct Held {
        Socket socket;
        sockaddr_in to;
        std::vector<std::uint8_t> bytes;
    };
    Simulated() {
        const char *text = std::getenv("D012_NET_SIM");
        if (text == nullptr || *text == '\0') return;
        double values[3] = {0.0, 0.0, 0.0};
        std::sscanf(text, "%lf,%lf,%lf", &values[0], &values[1], &values[2]);
        delay_ms_ = std::max(0.0, values[0]);
        jitter_ms_ = std::max(0.0, values[1]);
        loss_ = std::clamp(values[2], 0.0, 100.0);
        on_ = true;
        std::cout << "[net] simulating a path of " << delay_ms_ << " ms each way, give or take " << jitter_ms_
                  << " ms, losing " << loss_ << "% of packets (D012_NET_SIM)" << std::endl;
        std::thread([this] { run(); }).detach();
    }
    void run() {
        std::unique_lock lock(mutex_);
        for (;;) {
            if (held_.empty()) {
                wake_.wait(lock);
                continue;
            }
            const auto first = held_.begin();
            if (first->first > Clock::now()) {
                wake_.wait_until(lock, first->first);
                continue;
            }
            const Held packet = first->second;
            held_.erase(first);
            lock.unlock();
            send_now(packet.socket, packet.to, packet.bytes);
            lock.lock();
        }
    }
    bool on_ = false;
    double delay_ms_ = 0.0, jitter_ms_ = 0.0, loss_ = 0.0;
    std::mt19937 random_{12345u};
    std::mutex mutex_;
    std::condition_variable wake_;
    std::multimap<Clock::time_point, Held> held_;
};

void send(const Link &l, const std::vector<std::uint8_t> &out) {
    Simulated &sim = Simulated::get();
    if (sim.on()) sim.send(l.socket, l.peer, out);
    else send_now(l.socket, l.peer, out);
}

void send_welcome(const Link &l) {
    std::vector<std::uint8_t> out = packet(l, kWelcome, kHead + 17u + l.answers.size());
    put32(out, kHead, kVersion);
    put32(out, kHead + 4u, l.delay);
    put64(out, kHead + 8u, l.epoch);
    out[kHead + 16u] = static_cast<std::uint8_t>(l.answers.size());
    std::memcpy(out.data() + kHead + 17u, l.answers.data(), l.answers.size());
    send(l, out);
}

void send_input(Link &l) {
    const std::uint64_t end = l.mine.empty() ? 0u : l.mine.rbegin()->first + 1u;
    const std::uint64_t first = std::min(end, std::max<std::uint64_t>(l.confirmed, l.delay));
    const std::uint64_t count = std::min(end - first, kMaxCarried);
    std::vector<std::uint8_t> out = packet(l, kInput, kInputHead + count * kLocalSize);
    put64(out, kHead, l.their_count);
    put64(out, kHead + 8u, first);
    put16(out, kHead + 16u, static_cast<std::uint16_t>(count));
    put64(out, kHead + 24u, l.hash_read);
    put64(out, kHead + 32u, l.hash);
    put32(out, kHead + 40u, clock_ms());
    std::uint32_t echo = kNoTime;
    if (l.their_time != kNoTime)
        echo = l.their_time + static_cast<std::uint32_t>(
                                  std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - l.their_time_at)
                                      .count());
    put32(out, kHead + 44u, echo);
    for (std::uint64_t i = 0; i < count; ++i) {
        const Local &m = l.mine.at(first + i);
        const std::size_t at = kInputHead + i * kLocalSize;
        out[at] = m.dialog;
        out[at + 1u] = m.save;
        out[at + 2u] = m.pad.connected ? 1u : 0u;
        out[at + 3u] = m.pad.lx;
        out[at + 4u] = m.pad.ly;
        out[at + 5u] = m.pad.rx;
        out[at + 6u] = m.pad.ry;
        put32(out, at + 8u, m.pad.buttons);
        put32(out, at + 12u, m.pad.extra);
    }
    send(l, out);
    l.sent = Clock::now();
}

// Both copies' hashes of the same frame, once both are in.
void compare_hashes(Link &l) {
    while (!l.my_hashes.empty() && !l.their_hashes.empty()) {
        const auto mine = l.my_hashes.begin();
        const auto theirs = l.their_hashes.begin();
        if (mine->first < theirs->first) {
            l.my_hashes.erase(mine);
            continue;
        }
        if (theirs->first < mine->first) {
            l.their_hashes.erase(theirs);
            continue;
        }
        const std::uint64_t read = mine->first;
        if (mine->second != theirs->second && !l.apart) {
            l.apart = true;
            std::cerr << "[" << l.name << "] the two copies went apart: their hashes differ at " << l.unit << " "
                      << read << std::endl;
        } else if (!l.apart && (l.checks++ % 64u) == 0u) {
            std::cout << "[" << l.name << "] in step at " << l.unit << " " << read << std::endl;
        }
        l.my_hashes.erase(mine);
        l.their_hashes.erase(theirs);
    }
}

void take_input(Link &l, const std::uint8_t *data, std::size_t size) {
    if (size < kInputHead) return;
    const std::uint64_t holds = get<std::uint64_t>(data + kHead);
    const std::uint64_t first = get<std::uint64_t>(data + kHead + 8u);
    const std::uint64_t count = get<std::uint16_t>(data + kHead + 16u);
    const std::uint64_t hash_read = get<std::uint64_t>(data + kHead + 24u);
    if (size < kInputHead + count * kLocalSize) return;
    l.confirmed = std::max(l.confirmed, holds);
    l.their_time = get<std::uint32_t>(data + kHead + 40u);
    l.their_time_at = Clock::now();
    if (const std::uint32_t echo = get<std::uint32_t>(data + kHead + 44u); echo != kNoTime) {
        const std::uint32_t trip = clock_ms() - echo;
        if (trip < 10000u) {
            ++l.trips;
            l.trip_total += trip;
            l.trip_least = std::min(l.trip_least, trip);
            l.trip_most = std::max(l.trip_most, trip);
        }
    }
    for (std::uint64_t i = 0; i < count; ++i) {
        const std::uint64_t frame = first + i;
        if (frame < l.their_count || l.theirs.count(frame) != 0u) continue;
        const std::uint8_t *in = data + kInputHead + i * kLocalSize;
        Local m;
        m.dialog = in[0];
        m.save = in[1];
        m.pad.connected = in[2] != 0u;
        m.pad.lx = in[3];
        m.pad.ly = in[4];
        m.pad.rx = in[5];
        m.pad.ry = in[6];
        m.pad.buttons = get<std::uint32_t>(in + 8u);
        m.pad.extra = get<std::uint32_t>(in + 12u);
        l.theirs[frame] = m;
    }
    while (l.theirs.count(l.their_count) != 0u) ++l.their_count;
    if (hash_read != kNoRead) {
        l.their_hashes[hash_read] = get<std::uint64_t>(data + kHead + 32u);
        compare_hashes(l);
    }
}

bool same_address(const sockaddr_in &a, const sockaddr_in &b) {
    return a.sin_addr.s_addr == b.sin_addr.s_addr && a.sin_port == b.sin_port;
}

void handle(Link &l, const std::uint8_t *data, std::size_t size, const sockaddr_in &from) {
    if (size < kHead || std::memcmp(data, kMagic, 4u) != 0 || tag_of(data) != l.tag) return;
    const std::uint8_t kind = data[4];
    if (kind == kHello && l.player1 && size >= kHead + 4u) {
        if (get<std::uint32_t>(data + kHead) != kVersion) {
            std::cerr << "[" << l.name << "] a copy with another version of the link asked to join; ignored"
                      << std::endl;
            return;
        }
        if (!l.peer_known) {
            l.peer = from;
            l.peer_known = true;
            l.heard = Clock::now();
        }
        if (same_address(from, l.peer)) send_welcome(l);
        return;
    }
    if (!l.peer_known || !same_address(from, l.peer)) return;
    l.heard = Clock::now();
    if (kind == kWelcome && !l.player1 && !l.welcomed && size >= kHead + 17u) {
        const std::size_t length = data[kHead + 16u];
        if (get<std::uint32_t>(data + kHead) != kVersion || size < kHead + 17u + length) return;
        l.delay = get<std::uint32_t>(data + kHead + 4u);
        l.epoch = get<std::uint64_t>(data + kHead + 8u);
        l.answers.assign(reinterpret_cast<const char *>(data + kHead + 17u), length);
        l.welcomed = true;
        return;
    }
    if (kind == kInput) take_input(l, data, size);
    if (kind == kBye && !l.left) {
        l.left = true;
        l.left_at = Clock::now();
        std::cout << "[" << l.name << "] the other copy left" << std::endl;
    }
}

// Takes in every packet waiting, after waiting up to `wait` for the first.
void receive(Link &l, std::chrono::milliseconds wait) {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(l.socket, &set);
    timeval time{};
    time.tv_sec = static_cast<long>(wait.count() / 1000);
    time.tv_usec = static_cast<long>((wait.count() % 1000) * 1000);
    if (select(static_cast<int>(l.socket + 1), &set, nullptr, nullptr, &time) <= 0) return;
    for (;;) {
        std::uint8_t data[2048];
        sockaddr_in from{};
        AddressSize from_size = sizeof from;
        const int got = static_cast<int>(recvfrom(l.socket, reinterpret_cast<char *>(data), sizeof data, 0,
                                                  reinterpret_cast<sockaddr *>(&from), &from_size));
        if (got < 0) return;  // nothing more waiting (or an error a later read will meet again)
        handle(l, data, static_cast<std::size_t>(got), from);
    }
}

void set_env(const char *name, const std::string &value) {
#if defined(_WIN32)
    _putenv_s(name, value.c_str());
#else
    setenv(name, value.c_str(), 1);
#endif
}

bool parse_address(const std::string &text, sockaddr_in &out) {
    out = sockaddr_in{};
    out.sin_family = AF_INET;
    const std::size_t colon = text.rfind(':');
    const std::string host = colon == std::string::npos ? "127.0.0.1" : text.substr(0, colon);
    const std::string port = colon == std::string::npos ? text : text.substr(colon + 1u);
    const long number = std::strtol(port.c_str(), nullptr, 10);
    if (number <= 0 || number > 65535) return false;
    out.sin_port = htons(static_cast<std::uint16_t>(number));
    return inet_pton(AF_INET, host == "localhost" ? "127.0.0.1" : host.c_str(), &out.sin_addr) == 1;
}

bool open_socket(Link &l, const sockaddr_in &bound) {
#if defined(_WIN32)
    static const bool started = [] {
        WSADATA wsa{};
        return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
    }();
    if (!started) return false;
#endif
    l.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (l.socket == kNoSocket) return false;
#if defined(_WIN32)
    u_long on = 1u;
    ioctlsocket(l.socket, FIONBIO, &on);
    // A datagram to a port nobody listens on yet (player 1 not started) must
    // not make the next receive fail.
    constexpr DWORD kUdpConnReset = 0x9800000Cu;  // SIO_UDP_CONNRESET
    BOOL report = FALSE;
    DWORD bytes = 0;
    WSAIoctl(l.socket, kUdpConnReset, &report, sizeof report, nullptr, 0, &bytes, nullptr, nullptr);
#else
    fcntl(l.socket, F_SETFL, fcntl(l.socket, F_GETFL, 0) | O_NONBLOCK);
#endif
    if (bind(l.socket, reinterpret_cast<const sockaddr *>(&bound), sizeof bound) != 0) {
        close_socket(l.socket);
        l.socket = kNoSocket;
        return false;
    }
    return true;
}

std::uint32_t env_number(const char *name, std::uint32_t fallback, std::uint32_t most) {
    const char *text = std::getenv(name);
    if (text == nullptr || *text == '\0') return fallback;
    return static_cast<std::uint32_t>(std::min<unsigned long>(std::strtoul(text, nullptr, 10), most));
}

// Opens `l` and waits up to D012_NET_WAIT seconds for the other copy: as
// player 1 listening on `address`, else joining it. Player 1's copy sets the
// input delay (D012_NET_DELAY), and its epoch and answers, which the other
// takes.
bool connect(Link &l, bool host, const sockaddr_in &address, const std::string &where) {
    const char *name = l.name;
    const char *unit = l.unit;
    const std::uint32_t tag = l.tag;
    const std::uint64_t epoch = l.epoch;
    const std::string answers = l.answers;
    l = Link{};
    l.name = name;
    l.unit = unit;
    l.tag = tag;
    l.epoch = epoch;
    l.answers = answers;
    l.player1 = host;
    sockaddr_in bound{};
    bound.sin_family = AF_INET;
    if (host) {
        bound = address;
    } else {
        // Joining a copy on this machine needs no address but the loopback one,
        // and binding only that asks nothing of the firewall.
        const bool loopback = (ntohl(address.sin_addr.s_addr) >> 24) == 127u;
        bound.sin_addr.s_addr = htonl(loopback ? INADDR_LOOPBACK : INADDR_ANY);
        l.peer = address;
        l.peer_known = true;
    }
    if (!open_socket(l, bound)) {
        std::cerr << "[" << l.name << "] cannot open a UDP socket for " << where << std::endl;
        return false;
    }
    const auto wait = std::chrono::seconds(env_number("D012_NET_WAIT", 300u, 3600u));
    const auto began = Clock::now();
    if (host) {
        const std::string delay = setting("D012_NET_DELAY", "online.input_delay");
        l.delay = delay.empty() ? 3u
                                : static_cast<std::uint32_t>(
                                      std::min<unsigned long>(std::strtoul(delay.c_str(), nullptr, 10), 30u));
        std::cout << "[" << l.name << "] waiting for the other copy on " << where << std::endl;
        while (!l.peer_known && Clock::now() - began < wait) receive(l, std::chrono::milliseconds(100));
    } else {
        std::cout << "[" << l.name << "] joining the other copy on " << where << std::endl;
        auto hello = Clock::now() - kHelloEvery;
        while (!l.welcomed && Clock::now() - began < wait) {
            if (Clock::now() - hello >= kHelloEvery) {
                std::vector<std::uint8_t> out = packet(l, kHello, kHead + 4u);
                put32(out, kHead, kVersion);
                send(l, out);
                hello = Clock::now();
            }
            receive(l, std::chrono::milliseconds(50));
        }
    }
    if (!l.peer_known || (!host && !l.welcomed)) {
        std::cerr << "[" << l.name << "] the other copy did not come" << std::endl;
        close_socket(l.socket);
        l.socket = kNoSocket;
        return false;
    }
    l.their_count = l.delay;
    l.confirmed = l.delay;
    l.heard = Clock::now();
    l.active = true;
    return true;
}

// This copy's input for `frame` goes out for frame + delay; returns this
// copy's and the other copy's input for `frame` once the other's is in, or
// the other's as not connected if it is lost.
std::pair<Local, Local> trade(Link &l, std::uint64_t frame, const Local &local) {
    l.mine[frame + l.delay] = local;
    send_input(l);
    if (frame >= l.delay && !l.lost && l.their_count <= frame) {
        const auto began = Clock::now();
        while (l.their_count <= frame) {
            receive(l, std::chrono::milliseconds(2));
            const auto now = Clock::now();
            if (now - l.sent >= kResend) send_input(l);
            // After a BYE, its last frames may still be on their way (packets
            // can pass each other): a moment for them.
            if ((l.left && now - l.left_at >= kLastFrames) || now - l.heard >= kLost) {
                l.lost = true;
                std::cerr << "[" << l.name << "] lost the other copy at " << l.unit << " " << frame << std::endl;
                break;
            }
        }
        const auto waited = Clock::now() - began;
        ++l.waits;
        l.waited += waited;
        l.longest = std::max(l.longest, waited);
        if (waited >= kLongWait && !l.lost)
            std::cout << "[" << l.name << "] waited "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(waited).count()
                      << " ms for the other copy at " << l.unit << " " << frame << std::endl;
    } else {
        receive(l, std::chrono::milliseconds(0));
    }
    const Local me = frame < l.delay ? idle() : l.mine.at(frame);
    Local them = idle();
    if (frame >= l.delay) {
        if (const auto at = l.theirs.find(frame); at != l.theirs.end()) {
            them = at->second;
            l.theirs.erase(at);
        } else {
            them.pad.connected = false;  // lost
        }
    }
    l.mine.erase(l.mine.begin(), l.mine.lower_bound(std::min(l.confirmed, frame + 1u)));
    if (l.report_from == 0u && frame == 0u) l.report_began = Clock::now();
    if (frame + 1u - l.report_from >= kReportEvery) {
        const auto ms = [](Clock::duration d) {
            return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
        };
        const auto now = Clock::now();
        std::cout << "[" << l.name << "] " << l.unit << "s " << l.report_from << "-" << frame << " in "
                  << ms(now - l.report_began) << " ms: waited on " << l.waits << ", " << ms(l.waited)
                  << " ms in all, the longest " << ms(l.longest) << " ms";
        if (l.trips != 0u)
            std::cout << "; round trip " << l.trip_total / l.trips << " ms on average, " << l.trip_least << " to "
                      << l.trip_most;
        std::cout << std::endl;
        l.trips = 0u;
        l.trip_total = 0u;
        l.trip_least = kNoTime;
        l.trip_most = 0u;
        l.report_began = now;
        l.report_from = frame + 1u;
        l.waits = 0u;
        l.waited = {};
        l.longest = {};
    }
    return {me, them};
}

void note_hash(Link &l, std::uint64_t frame, std::uint64_t hash) {
    l.hash_read = frame;
    l.hash = hash;
    l.my_hashes[frame] = hash;
    while (l.my_hashes.size() > 256u) l.my_hashes.erase(l.my_hashes.begin());
    while (l.their_hashes.size() > 256u) l.their_hashes.erase(l.their_hashes.begin());
    compare_hashes(l);
}

void shut(Link &l) {
    if (l.socket != kNoSocket) close_socket(l.socket);
    l.socket = kNoSocket;
    l.active = false;
}

} // namespace

std::string setting(const char *env, const char *key) {
    if (const char *text = std::getenv(env); text != nullptr && *text != '\0') return text;
    try {
        const auto entries =
            portablekit::install::read_settings_file(portablekit::install::user_data_directory());
        if (const auto found = entries.find(key); found != entries.end()) return found->second;
    } catch (const std::exception &e) {
        std::cerr << "[net] settings.ini not read for " << key << ": " << e.what() << std::endl;
    }
    return {};
}

bool start() {
    Link &l = link();
    const char *setting = std::getenv("D012_NET");
    if (setting == nullptr || *setting == '\0') return false;
    const std::string text(setting);
    const bool host = text.rfind("host:", 0) == 0;
    const bool join = text.rfind("join:", 0) == 0;
    sockaddr_in address{};
    if ((!host && !join) || !parse_address(text.substr(5u), address)) {
        std::cerr << "[net] D012_NET is host:PORT, host:ADDRESS:PORT or join:ADDRESS:PORT, not \"" << text << "\""
                  << std::endl;
        return false;
    }
    if (host) {
        const char *epoch = std::getenv("D012_SYNC_EPOCH");
        l.epoch = epoch != nullptr && *epoch != '\0' ? std::strtoull(epoch, nullptr, 10)
                                                      : static_cast<std::uint64_t>(std::time(nullptr));
        const char *answers = std::getenv("D012_SAVEDATA_ANSWERS");
        l.answers = answers != nullptr && *answers != '\0' ? std::string(answers).substr(0u, 255u) : "first";
    }
    if (!connect(l, host, address, text.substr(5u))) {
        std::cerr << "[net] playing alone" << std::endl;
        return false;
    }
    // Both copies' clocks and title-screen save choice, as player 1's copy has them.
    set_env("D012_SYNC_EPOCH", std::to_string(l.epoch));
    set_env("D012_NO_BUSY_CLOCK", "1");
    set_env("D012_SAVEDATA_ANSWERS", l.answers);
    std::cout << "[net] linked as player " << (host ? 1 : 2) << ": " << l.delay << " frames of input delay, clock "
              << l.epoch << ", saves " << l.answers << std::endl;
    return true;
}

bool active() { return link().active; }

bool player1() { return link().player1; }

input::Frame exchange(std::uint64_t read, const Local &local) {
    Link &l = link();
    const auto [me, them] = trade(l, read, local);
    const Local &first = l.player1 ? me : them;
    const Local &second = l.player1 ? them : me;
    input::Frame f;
    f.read = read;
    f.dialog = first.dialog;
    f.save = first.save;
    f.player1 = first.pad;
    f.player2 = second.pad;
    return f;
}

void check(std::uint64_t read, std::uint64_t hash) { note_hash(link(), read, hash); }

namespace battle {

bool configured() { return !setting("D012_NET_BATTLE", "online.battle_link").empty(); }

bool open(int player, std::uint32_t tag) {
    Link &l = battle_link();
    shut(l);
    const std::string text = setting("D012_NET_BATTLE", "online.battle_link");
    sockaddr_in address{};
    if (!parse_address(text, address)) {
        std::cerr << "[battle-net] D012_NET_BATTLE is PORT or ADDRESS:PORT, not \"" << text << "\"" << std::endl;
        return false;
    }
    l.name = "battle-net";
    l.unit = "frame";
    l.tag = tag & kTagMask;
    if (!connect(l, player == 0, address, text)) return false;
    std::cout << "[battle-net] linked as player " << player << " for battle " << l.tag << ": " << l.delay
              << " frames of input delay" << std::endl;
    return true;
}

bool open_now() { return battle_link().active; }

Pads exchange(std::uint64_t frame, const input::Pad &mine) {
    Link &l = battle_link();
    Local local;
    local.pad = mine;
    const auto [me, them] = trade(l, frame, local);
    Pads pads;
    pads.player[l.player1 ? 0 : 1] = me.pad;
    pads.player[l.player1 ? 1 : 0] = them.pad;
    pads.lost = l.lost;
    return pads;
}

bool check(std::uint64_t frame, std::uint64_t hash) {
    Link &l = battle_link();
    note_hash(l, frame, hash);
    return !l.apart;
}

void close() {
    Link &l = battle_link();
    if (l.active && !l.lost && !l.left) {
        // A battle that ends normally ends on the same frame on both copies,
        // and the other may still want this copy's last frames: they go again
        // first. Each is said a few times, since a datagram may be lost.
        for (int i = 0; i < 3; ++i) send_input(l);
        const std::vector<std::uint8_t> out = packet(l, kBye, kHead);
        for (int i = 0; i < 3; ++i) send(l, out);
    }
    shut(l);
}

} // namespace battle

} // namespace d012::net
