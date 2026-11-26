// Online play: delay-based lockstep over UDP (d012_net.hpp).
//
// Packets (little endian) start "D0NL", u8 kind, then a 24-bit link tag (0 for
// the link from start-up; a battle's own for a battle link), so a packet left
// over from an earlier battle is never taken for this one:
// - HELLO (1), the joining copy to the listening one: u32 version, then ABOUT.
// - WELCOME (2), the listening copy's answer: u32 version, u32 input delay,
//   u64 the PSP clock's epoch, u8 length and the save answers, u8 the path (1
//   direct, 0 the one HELLO came on), then ABOUT.
// - ABOUT: this copy's camera settings (f32 dead zone, f32 sensitivity, u8
//   inverts: 1 X, 2 Y), u8 count, then each must-match item's what and value
//   (u8 length and bytes each; battle::must_match()).
// - REFUSE (7), the listening copy's answer to a HELLO of another version or
//   with other must-match values: u8 length and what differs, for the player.
// - INPUT (3): u64 how many of the other copy's frames this copy holds (all
//   frames below it), u64 the first frame carried, u16 their count, u16
//   spare, u64 a checked read and u64 its hash (read ~0 for none), u32 this
//   copy's clock in ms and u32 the other copy's clock from its latest INPUT
//   plus how long this copy has held it (~0 for none: the round trip is the
//   sender's clock less that echo), then 16 bytes a frame: u8 dialog, u8 save,
//   u8 connected, u8 lx, ly, rx, ry, s8 mouse pitch, u32 buttons, u32 extra
//   (its low 16 bits; the high 16, an s16, the mouse's yaw: input::Pad).
// - BYE (4): this copy has left (a battle link closed by a quit); nothing more
//   will come.
// - PROBE (5), the listening copy to the joining one before its WELCOME: u32
//   its clock in ms; PROBE ECHO (6) sends the same u32 straight back. With
//   the input delay on auto (online.input_delay empty or "auto"), a few round
//   trips timed so decide the delay the WELCOME gives.
// - WATCH (9), a watching copy to the relay, which passes it to both fighters:
//   u32 the watcher's id, u64 how many of the battle's frames it holds (all
//   frames below it).
// - STREAM (10), the leading fighter to the relay, which passes it to the
//   battle's watchers: u64 the first frame carried, u16 their count, u8 1 once
//   the battle has ended for the leader, u8 the check values carried, u32 the
//   frames the leader holds (its battle ended there, once ended), u32 a hash
//   of the must-match list, both players'
//   camera settings (as in ABOUT, 9 bytes each), the check values (u32 frame,
//   u32 value each), then 32 bytes a frame: player 0's pad and player 1's, as
//   INPUT's 16 bytes with dialog and save 0.
// - ASK (12) and INTRO (13), a fighter and the relay (d012_battle_relay.hpp):
//   an introduction for a direct battle.
// - PUNCH (14), the joining copy straight to the other's addresses from INTRO:
//   nothing but the head. It opens this copy's router to the other's packets.
// Each INPUT carries every frame the other copy has not confirmed (up to 64),
// so a lost packet costs only the wait for the next one. The first `delay`
// frames carry nobody's input on either copy.
//
// Direct battles (docs/port/online-matchmaking.md, section 13): a relayed
// link asks the relay for an introduction (ASK) as it starts. Given the other
// copy's addresses (INTRO), the joining copy sends PUNCH to them, and the
// leading copy PROBE to them as well as through the relay; the other answers
// each PROBE on the path it came by. The battle goes direct when the direct
// path answered (WELCOME says so), else through the relay, before its first
// frame. A copy that waits a second for input with nothing on the direct path
// moves to the relay, and the other follows when INPUT comes that way.
// online.always_relay (D012_NET_ALWAYS_RELAY), Always through the server,
// asks for no introduction, so neither copy is given the other's address.
//
// The battle link's address and the input delay can also come from
// settings.ini (online.battle_link, online.input_delay), so that a copy needs
// no launch options. Without a battle link address, the copy that hosts the
// game's session listens, and the other joins the session's address
// (battle::open()).
//
// D012_NET_SIM=DELAY_MS[,JITTER_MS[,LOSS_PERCENT[,EVERY_S,OUTAGE_MS]]] is a
// test switch: every packet this copy sends is held DELAY_MS give or take
// JITTER_MS (so packets may arrive out of order), or dropped LOSS_PERCENT
// times in a hundred, as an internet path would, and every one dropped for
// the last OUTAGE_MS of every EVERY_S seconds. Two copies on one PC each with
// that setting are a link with twice DELAY_MS of round trip.
// D012_NET_SIM_STREAM=1 keeps that to the STREAM packets, the watchers' path.
// D012_NET_DIRECT_CUT=SECONDS, another: that long into a battle, this copy
// stops sending and taking anything on the direct path, as a router that
// dropped it would.
//
// MIT licence; see port/README.md.

#if defined(_WIN32) && !defined(NOMINMAX)
#define NOMINMAX  // <windows.h>'s min and max macros break std::min under MSVC
#endif

#include "d012_net.hpp"

#include "d012_battle_relay.hpp"
#include "d012_frame_balance.hpp"
#include "d012_where.hpp"

#include "adhoc/session.hpp"

#include "install/user_data.hpp"
#include "portablekit/hle_extension.hpp"
#if defined(PORTABLEKIT_HAS_RENDERER)
#include "ui/message_dialog.hpp"
#endif

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
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
constexpr std::uint8_t kProbe = 5u;
constexpr std::uint8_t kProbeEcho = 6u;
constexpr std::uint8_t kRefuse = 7u;
constexpr std::uint8_t kHere = 8u;  // the leading copy, to the relay: "I am here"
constexpr std::uint8_t kWatch = 9u;
constexpr std::uint8_t kStream = 10u;
constexpr std::uint8_t kPunch = 14u;  // 12 and 13 are the relay's ASK and INTRO
constexpr std::uint32_t kVersion = 9u;
constexpr std::size_t kHead = 8u;
constexpr std::size_t kInputHead = kHead + 52u;
constexpr std::size_t kLocalSize = 16u;
constexpr std::uint64_t kMaxCarried = 64u;
constexpr std::uint64_t kNoRead = ~0ull;
constexpr std::uint32_t kTagMask = 0xFFFFFFu;
constexpr std::uint32_t kNoTime = ~0u;
constexpr auto kResend = std::chrono::milliseconds(20);
constexpr auto kHelloEvery = std::chrono::milliseconds(250);
constexpr auto kLost = std::chrono::seconds(10);    // nothing heard from the other copy
constexpr auto kGiveUp = std::chrono::seconds(30);  // a frame of the other copy's that never comes
constexpr auto kQuietWait = std::chrono::milliseconds(500);  // a wait this long gets a screen
constexpr auto kLongWait = std::chrono::milliseconds(100);
constexpr auto kLastFrames = std::chrono::milliseconds(1000);
constexpr std::uint64_t kReportEvery = 900u;  // frames between reports of the waits
constexpr std::size_t kProbes = 8u;           // round trips timed for an automatic delay
constexpr auto kProbeEvery = std::chrono::milliseconds(40);
constexpr auto kProbeFor = std::chrono::seconds(2);
constexpr std::size_t kDirectLeast = 3u;      // the direct path's answers that make it the battle's
constexpr std::uint32_t kDirectSlack = 5u;    // ms the direct path may be slower and still be chosen
constexpr auto kDirectQuiet = std::chrono::seconds(1);  // waiting, nothing direct: move to the relay
constexpr auto kKeepRelay = std::chrono::seconds(5);    // ASK again while direct
constexpr std::uint16_t kBattlePort = 27420;  // the battle link's UDP port by default
// The watchers' stream: the most frames a STREAM carries (it stays under a
// path's usual 1,500-byte limit), the check values it carries, how often it
// goes when there is nothing new, and how long the leading copy keeps
// sending as its battle ends until every watcher holds it all.
constexpr std::size_t kStreamHead = kHead + 38u;
constexpr std::uint64_t kStreamMost = 36u;
constexpr std::size_t kStreamChecks = 4u;
constexpr auto kStreamKeep = std::chrono::milliseconds(100);
constexpr auto kStreamFlush = std::chrono::milliseconds(1500);
// A watcher: how often it says what it holds, and how long it waits for a
// battle's first frames (the fighters may take 30 s to link).
constexpr auto kWatchEvery = std::chrono::milliseconds(30);
constexpr auto kWatchFirst = std::chrono::seconds(40);
// Its buffer, the frames it keeps in hand so a late packet does not stop its
// battle (30 frames a second). It changes its pace only to follow the
// fighters', whose frames come a little slower at times as they keep in step
// with each other: more than kWatchDrift frames short all through the last
// kWatchDriftLook frames or more (not a gap in the path, which the buffer is
// for), each frame is held back kWatchHold, as little as the fighters' own
// hold (FrameBalance::kMostHold). The
// game's sound is timed by its frames, and a larger change would break it up.
// A frame that has not come waits until the whole buffer is in again. Such a
// wait the path caused (not the fighters' own) makes the buffer as many
// frames larger as the wait lasted, and kWatchRaise more: the frames that
// came in the wait are the buffer then, so nothing is skipped after it.
// kWatchWindow frames without one in which a quarter of the buffer (at least
// kWatchMargin frames) was always in hand more than needed make it up to
// kWatchLower smaller. A quarter of the buffer more in hand than it (at least
// kWatchSkip frames) for kWatchLook frames on end (not a moment's bunch of
// late packets), as after a smaller buffer, are run unseen. After a slow moment of
// this copy's own (more than kWatchSlow between two frames) they are run
// unseen at once: the sound has just stopped for it, and a skip a moment later
// would stop it again.
constexpr std::uint64_t kWatchStart = 12u;
constexpr std::uint64_t kWatchLeast = 6u;
constexpr std::uint64_t kWatchMost = 45u;
constexpr std::uint64_t kWatchRaise = 3u;
constexpr std::uint64_t kWatchWindow = 900u;
constexpr std::uint64_t kWatchMargin = 4u;
constexpr std::uint64_t kWatchLower = 3u;
constexpr std::uint64_t kWatchSkip = 8u;
constexpr std::uint64_t kWatchLook = 15u;
// As the kernel's own limit: a debt larger than it is dropped, not made up,
// so the copy stays behind.
constexpr auto kWatchSlow = std::chrono::milliseconds(100);
constexpr std::uint64_t kWatchDrift = 2u;
constexpr std::uint64_t kWatchDriftLook = 90u;
constexpr auto kWatchHold = std::chrono::microseconds(1000);
constexpr auto kWatchFrameTime = std::chrono::microseconds(33367);  // the battle's frame: two vblanks

// Takes the link's datagrams as they come, on a thread of its own, with the
// moment each came: the game's thread reads the socket only once a frame, and
// the frames' balance (Link::frames) needs when an input really arrived.
class Receiver {
public:
    struct Datagram {
        std::vector<std::uint8_t> bytes;
        sockaddr_in from;
        Clock::time_point at;
    };
    explicit Receiver(Socket socket) : socket_(socket), thread_([this] { run(); }) {}
    ~Receiver() {
        stop_ = true;
        thread_.join();
    }
    Receiver(const Receiver &) = delete;
    Receiver &operator=(const Receiver &) = delete;
    // Every datagram that came, after waiting up to `wait` for the first.
    std::vector<Datagram> take(std::chrono::milliseconds wait) {
        std::unique_lock lock(mutex_);
        if (queue_.empty() && wait.count() > 0) came_.wait_for(lock, wait, [this] { return !queue_.empty(); });
        std::vector<Datagram> out;
        out.swap(queue_);
        return out;
    }

private:
    void run() {
        while (!stop_) {
            fd_set set;
            FD_ZERO(&set);
            FD_SET(socket_, &set);
            timeval time{0, 5000};
            if (select(static_cast<int>(socket_ + 1), &set, nullptr, nullptr, &time) <= 0) continue;
            for (;;) {
                std::uint8_t data[2048];
                sockaddr_in from{};
                AddressSize from_size = sizeof from;
                const int got = static_cast<int>(recvfrom(socket_, reinterpret_cast<char *>(data), sizeof data, 0,
                                                          reinterpret_cast<sockaddr *>(&from), &from_size));
                if (got < 0) break;  // nothing more waiting (or an error a later read will meet again)
                const Clock::time_point at = Clock::now();
                std::lock_guard lock(mutex_);
                queue_.push_back(Datagram{std::vector<std::uint8_t>(data, data + got), from, at});
                came_.notify_one();
            }
        }
    }
    Socket socket_;
    std::atomic<bool> stop_{false};
    std::mutex mutex_;
    std::condition_variable came_;
    std::vector<Datagram> queue_;
    std::thread thread_;
};

struct Link {
    const char *name = "net";  // the log's prefix
    const char *unit = "read"; // what its frames are, for the log
    bool active = false;
    bool player1 = false;
    bool peer_known = false;
    bool welcomed = false;
    bool decided = false;  // the leading copy: the delay is settled, so HELLOs get a WELCOME
    // Each copy's own input delay setting, 0 for auto; the joining copy sends
    // its own in HELLO. The higher counts, auto as the connection needs.
    std::uint32_t fixed = 0;
    std::uint32_t their_fixed = 0;
    std::vector<std::uint32_t> probe_trips;
    std::uint32_t probe_median = 0;  // ms, once settle_delay has timed some
    bool lost = false;
    bool leaving = false;  // this copy's player chose to leave while it waited
    bool refused = false;  // the other copy differs (REFUSE), or this one refused it
    std::string refusal;   // what differs, as the player reads it
    battle::Camera their_camera;
    bool left = false;  // the other copy said BYE
    Clock::time_point left_at{};
    bool apart = false;
    std::uint32_t tag = 0;
    std::uint32_t delay = 3u;
    std::uint64_t epoch = 0;
    std::string answers;
    Socket socket = kNoSocket;
    std::shared_ptr<Receiver> receiver;
    sockaddr_in peer{};
    std::optional<sockaddr_in> relay;  // the session host's relay, when the battle goes through it
    // A direct battle (a relayed link only): whether this copy may go direct
    // (not Always through the server), its address on its own network for
    // ASK, the relay's INTRO and the other copy's addresses it gave, the other
    // copy's address once something came straight from it, whether the
    // battle's packets go that way and when the last came, the latest ASK, the
    // leading copy's round trips on the direct path (probe_trips: the relay's),
    // and D012_NET_DIRECT_CUT's moment.
    bool direct_ok = false;
    sockaddr_in local{};
    bool introduced = false;
    bool intro_direct = false;
    std::vector<sockaddr_in> candidates;
    std::optional<sockaddr_in> direct;
    bool on_direct = false;
    Clock::time_point direct_heard{};
    Clock::time_point asked{};
    std::vector<std::uint32_t> direct_trips;
    Clock::time_point cut_at = Clock::time_point::max();
    std::map<std::uint64_t, Local> mine;    // by frame, from the oldest the other copy may still need
    std::map<std::uint64_t, Local> theirs;  // by frame, received and not yet used
    std::map<std::uint64_t, Clock::time_point> arrived;  // when each of `theirs` first came
    // Rollback (battle::exchange_ahead): this copy's input by frame, kept for
    // frames run again; the other's input this copy guessed, by frame, until
    // the real one comes; and the earliest frame whose guess was wrong.
    std::map<std::uint64_t, Local> history;
    std::map<std::uint64_t, Local> guessed;
    std::optional<std::uint64_t> wrong_from;
    // The frames' balance (d012_frame_balance.hpp): the copy that is ahead
    // holds its frames back a little until both have the same slack, so that
    // neither waits until a delay spike eats all of it and, with rollback,
    // neither has all the guessing (docs/port/online-next.md). `ran`: with
    // rollback, when each frame still waiting for the other's input ran.
    bool balance = true;
    FrameBalance frames;
    std::map<std::uint64_t, Clock::time_point> ran;
    Clock::duration held{};  // held back since the last report
    std::uint64_t their_count = 0;          // the other copy's frames held: all below this one
    std::uint64_t confirmed = 0;            // this copy's frames the other copy holds
    // The watchers' stream (the leading copy of a relayed battle): both
    // players' pads of every certain frame from the battle's first, by frame;
    // the check values sent; and the copies watching, by id.
    struct Watcher {
        std::uint64_t holds = 0;
        Clock::time_point heard{};
    };
    std::vector<std::pair<input::Pad, input::Pad>> stream;
    std::map<std::uint64_t, std::uint32_t> stream_checks;
    std::map<std::uint32_t, Watcher> watchers;
    Clock::time_point streamed{};
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

// What must match (battle::must_match) and this copy's camera settings
// (battle::set_camera): sent with HELLO and WELCOME.
std::vector<std::pair<std::string, std::string>> &matched() {
    static std::vector<std::pair<std::string, std::string>> items;
    return items;
}

battle::Camera &my_camera() {
    static battle::Camera camera;
    return camera;
}

// After HELLO's version and WELCOME's answers: f32 dead zone, f32
// sensitivity, u8 inverts (1 X, 2 Y), u8 count, then each item's what and
// value, each a u8 length and its bytes.
void put_about(std::vector<std::uint8_t> &out) {
    const battle::Camera &camera = my_camera();
    for (const float value : {camera.dead_zone, camera.sensitivity}) {
        std::uint32_t bits = 0;
        std::memcpy(&bits, &value, 4u);
        out.resize(out.size() + 4u);
        put32(out, out.size() - 4u, bits);
    }
    out.push_back(static_cast<std::uint8_t>((camera.invert_x ? 1u : 0u) | (camera.invert_y ? 2u : 0u)));
    const auto &items = matched();
    const std::size_t count = std::min<std::size_t>(items.size(), 255u);
    out.push_back(static_cast<std::uint8_t>(count));
    const auto put_text = [&](const std::string &text) {
        const std::size_t length = std::min<std::size_t>(text.size(), 255u);
        out.push_back(static_cast<std::uint8_t>(length));
        out.insert(out.end(), text.begin(), text.begin() + static_cast<std::ptrdiff_t>(length));
    };
    for (std::size_t i = 0; i < count; ++i) {
        put_text(items[i].first);
        put_text(items[i].second);
    }
}

struct About {
    battle::Camera camera;
    std::vector<std::pair<std::string, std::string>> items;
};

std::optional<About> take_about(const std::uint8_t *data, std::size_t size) {
    if (size < 10u) return std::nullopt;
    About about;
    float values[2] = {};
    for (int i = 0; i < 2; ++i) {
        const std::uint32_t bits = get<std::uint32_t>(data + 4u * static_cast<std::size_t>(i));
        std::memcpy(&values[i], &bits, 4u);
    }
    // As the camera module bounds its own settings.
    about.camera.dead_zone = std::isfinite(values[0]) ? std::clamp(values[0], 0.0f, 0.9f) : 0.18f;
    about.camera.sensitivity = std::isfinite(values[1]) ? std::clamp(values[1], 0.05f, 4.0f) : 1.0f;
    about.camera.invert_x = (data[8] & 1u) != 0u;
    about.camera.invert_y = (data[8] & 2u) != 0u;
    const std::size_t count = data[9];
    std::size_t at = 10u;
    const auto take_text = [&](std::string &text) {
        if (at >= size) return false;
        const std::size_t length = data[at++];
        if (at + length > size) return false;
        text.assign(reinterpret_cast<const char *>(data + at), length);
        at += length;
        return true;
    };
    for (std::size_t i = 0; i < count; ++i) {
        std::string what, value;
        if (!take_text(what) || !take_text(value)) return std::nullopt;
        about.items.emplace_back(std::move(what), std::move(value));
    }
    return about;
}

// What differs between this copy's list and `theirs`, for the player: "the
// port's version and mods that change the game".
std::string differences(const std::vector<std::pair<std::string, std::string>> &theirs) {
    const auto &mine = matched();
    std::vector<std::string> names;
    for (const auto &[what, value] : mine) {
        const auto found =
            std::find_if(theirs.begin(), theirs.end(), [&](const auto &item) { return item.first == what; });
        if (found == theirs.end() || found->second != value) names.push_back(what);
    }
    for (const auto &item : theirs)
        if (std::none_of(mine.begin(), mine.end(), [&](const auto &m) { return m.first == item.first; }))
            names.push_back(item.first);
    std::string text;
    for (std::size_t i = 0; i < names.size(); ++i)
        text += (i == 0u ? "" : i + 1u == names.size() ? " and " : ", ") + names[i];
    return text;
}

// Milliseconds on this copy's clock, now or at `at`.
std::uint32_t clock_ms(Clock::time_point at = Clock::now()) {
    static const Clock::time_point start = Clock::now();
    return static_cast<std::uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(at - start).count());
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
        if (stream_only_ && (out.size() <= 4u || out[4] != kStream)) {
            send_now(socket, to, out);
            return;
        }
        std::lock_guard lock(mutex_);
        if (std::uniform_real_distribution<double>(0.0, 100.0)(random_) < loss_) return;
        if (outage_every_.count() > 0 && outage_.count() > 0 &&
            (Clock::now() - began_) % outage_every_ >= outage_every_ - outage_)
            return;
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
        double values[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
        std::sscanf(text, "%lf,%lf,%lf,%lf,%lf", &values[0], &values[1], &values[2], &values[3], &values[4]);
        delay_ms_ = std::max(0.0, values[0]);
        jitter_ms_ = std::max(0.0, values[1]);
        loss_ = std::clamp(values[2], 0.0, 100.0);
        outage_every_ = std::chrono::milliseconds(static_cast<long long>(std::max(0.0, values[3]) * 1000.0));
        outage_ = std::chrono::milliseconds(static_cast<long long>(std::max(0.0, values[4])));
        began_ = Clock::now();
        const char *stream = std::getenv("D012_NET_SIM_STREAM");
        stream_only_ = stream != nullptr && std::string(stream) == "1";
        on_ = true;
        std::cout << "[net] simulating a path of " << delay_ms_ << " ms each way, give or take " << jitter_ms_
                  << " ms, losing " << loss_ << "% of packets";
        if (outage_every_.count() > 0 && outage_.count() > 0)
            std::cout << ", and all for " << std::chrono::duration_cast<std::chrono::milliseconds>(outage_).count()
                      << " ms every " << values[3] << " s";
        if (stream_only_) std::cout << ", only the watchers' stream";
        std::cout << " (D012_NET_SIM)" << std::endl;
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
    Clock::duration outage_every_{}, outage_{};
    bool stream_only_ = false;
    Clock::time_point began_{};
    std::mt19937 random_{12345u};
    std::mutex mutex_;
    std::condition_variable wake_;
    std::multimap<Clock::time_point, Held> held_;
};

bool same_address(const sockaddr_in &a, const sockaddr_in &b);

bool via_relay(const Link &l, const sockaddr_in &at) { return l.relay && same_address(at, *l.relay); }

// D012_NET_DIRECT_CUT: the direct path has stopped, for this copy.
bool direct_cut(const Link &l) { return l.active && Clock::now() >= l.cut_at; }

// From the other copy straight, not through the relay: an address INTRO gave,
// or the one its packets came from (its router may have changed the port).
bool from_direct(const Link &l, const sockaddr_in &from) {
    if (!l.relay || via_relay(l, from)) return false;
    if (l.direct && l.direct->sin_addr.s_addr == from.sin_addr.s_addr) return true;
    return std::any_of(l.candidates.begin(), l.candidates.end(),
                       [&](const sockaddr_in &c) { return c.sin_addr.s_addr == from.sin_addr.s_addr; });
}

void send_to(const Link &l, const sockaddr_in &to, const std::vector<std::uint8_t> &out) {
    if (direct_cut(l) && !via_relay(l, to)) return;
    Simulated &sim = Simulated::get();
    if (sim.on()) sim.send(l.socket, to, out);
    else send_now(l.socket, to, out);
}

void send(const Link &l, const std::vector<std::uint8_t> &out) { send_to(l, l.peer, out); }

// On both paths while the battle goes direct: the other copy may have moved
// to the relay.
void send_both(const Link &l, const std::vector<std::uint8_t> &out) {
    send(l, out);
    if (l.on_direct && l.relay) send_to(l, *l.relay, out);
}

// ASK the relay for an introduction (d012_battle_relay.hpp).
void send_ask(Link &l) {
    if (!l.relay) return;
    const std::string text = relay::ask(l.tag, l.direct_ok, relay::Endpoint{l.local.sin_addr.s_addr, l.local.sin_port});
    send_to(l, *l.relay, std::vector<std::uint8_t>(text.begin(), text.end()));
    l.asked = Clock::now();
}

// The relay's INTRO: the other copy's addresses to try, or none.
void take_intro(Link &l, const std::uint8_t *data, std::size_t size) {
    relay::Introduction intro;
    if (l.introduced || l.active || !relay::read_introduction(data, size, intro)) return;
    l.introduced = true;
    l.intro_direct = intro.direct && l.direct_ok;
    if (!l.intro_direct) {
        std::cout << "[" << l.name << "] the relay keeps this battle"
                  << (l.direct_ok ? " (the other player chose Always through the server, or it is on the relay's computer)"
                                  : " (Always through the server)")
                  << std::endl;
        return;
    }
    for (const relay::Endpoint e : {intro.seen, intro.local}) {
        if (e.address == 0u || e.port == 0u) continue;
        sockaddr_in a{};
        a.sin_family = AF_INET;
        a.sin_addr.s_addr = e.address;
        a.sin_port = e.port;
        if (std::none_of(l.candidates.begin(), l.candidates.end(), [&](const sockaddr_in &c) { return same_address(c, a); }))
            l.candidates.push_back(a);
    }
    std::cout << "[" << l.name << "] introduced to the other copy: trying a direct path" << std::endl;
}

// The battle's packets go through the relay from now on.
void move_to_relay(Link &l, const char *why) {
    if (!l.on_direct || !l.relay) return;
    l.on_direct = false;
    l.peer = *l.relay;
    std::cout << "[" << l.name << "] " << why << ": on through the relay" << std::endl;
}

void send_welcome(const Link &l) {
    std::vector<std::uint8_t> out = packet(l, kWelcome, kHead + 18u + l.answers.size());
    put32(out, kHead, kVersion);
    put32(out, kHead + 4u, l.delay);
    put64(out, kHead + 8u, l.epoch);
    out[kHead + 16u] = static_cast<std::uint8_t>(l.answers.size());
    std::memcpy(out.data() + kHead + 17u, l.answers.data(), l.answers.size());
    out[kHead + 17u + l.answers.size()] = l.on_direct ? 1u : 0u;
    put_about(out);
    send_both(l, out);
}

void send_hello(const Link &l) {
    std::vector<std::uint8_t> out = packet(l, kHello, kHead + 8u);
    put32(out, kHead, kVersion);
    put32(out, kHead + 4u, l.fixed);
    put_about(out);
    send(l, out);
}

// The listening copy: the copy at `to` differs in `what`, so this battle does
// not start. Said to it a few times, since a datagram may be lost.
void refuse(Link &l, const sockaddr_in &to, const std::string &what) {
    std::vector<std::uint8_t> out = packet(l, kRefuse, kHead + 1u);
    const std::size_t length = std::min<std::size_t>(what.size(), 255u);
    out[kHead] = static_cast<std::uint8_t>(length);
    out.insert(out.end(), what.begin(), what.begin() + static_cast<std::ptrdiff_t>(length));
    Simulated &sim = Simulated::get();
    for (int i = 0; i < 3; ++i) {
        if (sim.on()) sim.send(l.socket, to, out);
        else send_now(l.socket, to, out);
    }
    if (!l.refused)
        std::cout << "[" << l.name << "] the other copy differs in " << what << "; this battle does not start"
                  << std::endl;
    l.refused = true;
    l.refusal = what;
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
    put16(out, kHead + 48u, static_cast<std::uint16_t>(l.frames.sent_slack()));
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
        out[at + 7u] = static_cast<std::uint8_t>(m.pad.mouse_pitch);
        put32(out, at + 8u, m.pad.buttons);
        put32(out, at + 12u,
              (m.pad.extra & 0xFFFFu) | static_cast<std::uint32_t>(static_cast<std::uint16_t>(m.pad.mouse_yaw)) << 16u);
    }
    send(l, out);
    l.sent = Clock::now();
    // A direct battle keeps its place at the relay, and the router's path to
    // it, for a move there.
    if (l.on_direct && l.sent - l.asked >= kKeepRelay) send_ask(l);
}

// A pad in INPUT's 16 bytes, dialog and save 0 (the watchers' stream).
void put_pad(std::vector<std::uint8_t> &out, std::size_t at, const input::Pad &pad) {
    out[at + 2u] = pad.connected ? 1u : 0u;
    out[at + 3u] = pad.lx;
    out[at + 4u] = pad.ly;
    out[at + 5u] = pad.rx;
    out[at + 6u] = pad.ry;
    out[at + 7u] = static_cast<std::uint8_t>(pad.mouse_pitch);
    put32(out, at + 8u, pad.buttons);
    put32(out, at + 12u,
          (pad.extra & 0xFFFFu) | static_cast<std::uint32_t>(static_cast<std::uint16_t>(pad.mouse_yaw)) << 16u);
}

input::Pad take_pad(const std::uint8_t *in) {
    input::Pad pad;
    pad.connected = in[2] != 0u;
    pad.lx = in[3];
    pad.ly = in[4];
    pad.rx = in[5];
    pad.ry = in[6];
    pad.mouse_pitch = static_cast<std::int8_t>(in[7]);
    pad.buttons = get<std::uint32_t>(in + 8u);
    const std::uint32_t extra = get<std::uint32_t>(in + 12u);
    pad.extra = extra & 0xFFFFu;
    pad.mouse_yaw = static_cast<std::int16_t>(static_cast<std::uint16_t>(extra >> 16u));
    return pad;
}

void put_camera(std::vector<std::uint8_t> &out, std::size_t at, const battle::Camera &camera) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &camera.dead_zone, 4u);
    put32(out, at, bits);
    std::memcpy(&bits, &camera.sensitivity, 4u);
    put32(out, at + 4u, bits);
    out[at + 8u] = static_cast<std::uint8_t>((camera.invert_x ? 1u : 0u) | (camera.invert_y ? 2u : 0u));
}

battle::Camera take_camera(const std::uint8_t *in) {
    battle::Camera camera;
    float values[2] = {};
    for (int i = 0; i < 2; ++i) {
        const std::uint32_t bits = get<std::uint32_t>(in + 4u * static_cast<std::size_t>(i));
        std::memcpy(&values[i], &bits, 4u);
    }
    camera.dead_zone = std::isfinite(values[0]) ? std::clamp(values[0], 0.0f, 0.9f) : 0.18f;
    camera.sensitivity = std::isfinite(values[1]) ? std::clamp(values[1], 0.05f, 4.0f) : 1.0f;
    camera.invert_x = (in[8] & 1u) != 0u;
    camera.invert_y = (in[8] & 2u) != 0u;
    return camera;
}

// The must-match list in 32 bits: a watcher whose list differs from the
// fighters' would watch another battle.
std::uint32_t matched_hash() {
    std::uint32_t hash = 2166136261u;
    const auto mix = [&](const std::string &text) {
        for (const char c : text) hash = (hash ^ static_cast<std::uint8_t>(c)) * 16777619u;
        hash = (hash ^ 0xFFu) * 16777619u;
    };
    for (const auto &[what, value] : matched()) {
        mix(what);
        mix(value);
    }
    return hash;
}

// The leading copy: the stream to the watchers heard from in the last
// kLost, from the first frame any of them lacks. Every frame while one lacks
// some; otherwise every kStreamKeep, for the check values and so the watchers
// know the battle goes on. `end`: the battle has ended for this copy there.
void send_stream(Link &l, std::optional<std::uint64_t> end = std::nullopt) {
    if (!l.player1 || !l.relay || l.socket == kNoSocket) return;
    const auto now = Clock::now();
    std::uint64_t first = l.stream.size();
    bool any = false;
    for (auto it = l.watchers.begin(); it != l.watchers.end();) {
        if (now - it->second.heard > kLost) {
            it = l.watchers.erase(it);
            continue;
        }
        any = true;
        first = std::min(first, it->second.holds);
        ++it;
    }
    if (!any) return;
    const std::uint64_t count = std::min<std::uint64_t>(l.stream.size() - first, kStreamMost);
    if (count == 0u && !end && now - l.streamed < kStreamKeep) return;
    std::vector<std::pair<std::uint64_t, std::uint32_t>> checks;
    for (auto it = l.stream_checks.rbegin(); it != l.stream_checks.rend() && checks.size() < kStreamChecks; ++it)
        checks.emplace_back(it->first, it->second);
    std::vector<std::uint8_t> out = packet(l, kStream, kStreamHead + checks.size() * 8u + count * 2u * kLocalSize);
    put64(out, kHead, first);
    put16(out, kHead + 8u, static_cast<std::uint16_t>(count));
    out[kHead + 10u] = end ? 1u : 0u;
    out[kHead + 11u] = static_cast<std::uint8_t>(checks.size());
    put32(out, kHead + 12u, static_cast<std::uint32_t>(end.value_or(l.stream.size())));
    put32(out, kHead + 16u, matched_hash());
    // The leading copy is the game's player 0.
    put_camera(out, kHead + 20u, my_camera());
    put_camera(out, kHead + 29u, l.their_camera);
    std::size_t at = kStreamHead;
    for (const auto &[frame, value] : checks) {
        put32(out, at, static_cast<std::uint32_t>(frame));
        put32(out, at + 4u, value);
        at += 8u;
    }
    for (std::uint64_t i = 0; i < count; ++i) {
        const auto &[zero, one] = l.stream[first + i];
        put_pad(out, at, zero);
        put_pad(out, at + kLocalSize, one);
        at += 2u * kLocalSize;
    }
    send_to(l, *l.relay, out);  // the watchers' path, whichever way the battle goes
    l.streamed = now;
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

bool same_input(const Local &a, const Local &b);

void take_input(Link &l, const std::uint8_t *data, std::size_t size, Clock::time_point at) {
    if (size < kInputHead) return;
    const std::uint64_t holds = get<std::uint64_t>(data + kHead);
    const std::uint64_t first = get<std::uint64_t>(data + kHead + 8u);
    const std::uint64_t count = get<std::uint16_t>(data + kHead + 16u);
    const std::uint64_t hash_read = get<std::uint64_t>(data + kHead + 24u);
    if (size < kInputHead + count * kLocalSize) return;
    l.confirmed = std::max(l.confirmed, holds);
    l.their_time = get<std::uint32_t>(data + kHead + 40u);
    l.their_time_at = at;
    l.frames.hear(static_cast<std::int16_t>(get<std::uint16_t>(data + kHead + 48u)));
    if (const std::uint32_t echo = get<std::uint32_t>(data + kHead + 44u); echo != kNoTime) {
        const std::uint32_t trip = clock_ms(at) - echo;
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
        m.pad.mouse_pitch = static_cast<std::int8_t>(in[7]);
        m.pad.buttons = get<std::uint32_t>(in + 8u);
        const std::uint32_t extra = get<std::uint32_t>(in + 12u);
        m.pad.extra = extra & 0xFFFFu;
        m.pad.mouse_yaw = static_cast<std::int16_t>(static_cast<std::uint16_t>(extra >> 16u));
        l.theirs[frame] = m;
        l.arrived.emplace(frame, at);
        if (const auto guess = l.guessed.find(frame); guess != l.guessed.end()) {
            if (!same_input(guess->second, m)) l.wrong_from = std::min(l.wrong_from.value_or(frame), frame);
            l.guessed.erase(guess);
        }
    }
    while (l.theirs.count(l.their_count) != 0u) ++l.their_count;
    if (hash_read != kNoRead) {
        l.their_hashes[hash_read] = get<std::uint64_t>(data + kHead + 32u);
        compare_hashes(l);
    }
}

bool same_input(const Local &a, const Local &b) {
    return a.dialog == b.dialog && a.save == b.save && a.pad.connected == b.pad.connected &&
           a.pad.buttons == b.pad.buttons && a.pad.extra == b.pad.extra && a.pad.lx == b.pad.lx &&
           a.pad.ly == b.pad.ly && a.pad.rx == b.pad.rx && a.pad.ry == b.pad.ry && a.pad.mouse_yaw == b.pad.mouse_yaw &&
           a.pad.mouse_pitch == b.pad.mouse_pitch;
}

bool same_address(const sockaddr_in &a, const sockaddr_in &b) {
    return a.sin_addr.s_addr == b.sin_addr.s_addr && a.sin_port == b.sin_port;
}

void handle(Link &l, const std::uint8_t *data, std::size_t size, const sockaddr_in &from, Clock::time_point at) {
    if (size < kHead || std::memcmp(data, kMagic, 4u) != 0 || tag_of(data) != l.tag) return;
    const std::uint8_t kind = data[4];
    const bool relayed = via_relay(l, from);
    if (kind == relay::kIntro) {
        if (relayed) take_intro(l, data, size);
        return;
    }
    const bool direct = from_direct(l, from);
    if (direct) {
        if (direct_cut(l)) return;
        // Its address as the other copy's packets show it, until the battle
        // has chosen its path.
        if (!l.on_direct) l.direct = from;
        l.direct_heard = at;
        if (kind == kPunch) return;
    }
    if (kind == kHello && l.player1 && size >= kHead + 4u) {
        // Another version of the link is another version of the port.
        const std::optional<About> about = get<std::uint32_t>(data + kHead) == kVersion && size >= kHead + 8u
                                               ? take_about(data + kHead + 8u, size - kHead - 8u)
                                               : std::nullopt;
        if (!about) {
            refuse(l, from, "the port's version");
            return;
        }
        if (const std::string differs = differences(about->items); !differs.empty()) {
            refuse(l, from, differs);
            return;
        }
        if (!l.peer_known) {
            l.peer = from;
            l.peer_known = true;
            l.heard = Clock::now();
            l.their_camera = about->camera;
            l.their_fixed = std::min<std::uint32_t>(get<std::uint32_t>(data + kHead + 4u), 30u);
        }
        // A HELLO comes through the relay until its copy is welcomed, whichever
        // path the battle takes.
        if ((same_address(from, l.peer) || relayed) && l.decided) send_welcome(l);
        return;
    }
    if (kind == kWatch && l.player1 && l.relay && same_address(from, *l.relay) && size >= kHead + 12u) {
        Link::Watcher &w = l.watchers[get<std::uint32_t>(data + kHead)];
        if (w.heard == Clock::time_point{})
            std::cout << "[" << l.name << "] a copy watches this battle (" << l.watchers.size() << " in all)"
                      << std::endl;
        w.holds = std::max(w.holds, get<std::uint64_t>(data + kHead + 4u));
        w.heard = at;
        return;
    }
    if (!(l.peer_known && same_address(from, l.peer)) && !relayed && !direct) return;
    l.heard = Clock::now();
    if (kind == kRefuse && !l.player1 && size >= kHead + 1u) {
        const std::size_t length = std::min<std::size_t>(data[kHead], size - kHead - 1u);
        const std::string what(reinterpret_cast<const char *>(data + kHead + 1u), length);
        if (!l.refused)
            std::cout << "[" << l.name << "] the other copy says this one differs in " << what
                      << "; this battle does not start" << std::endl;
        l.refused = true;
        l.refusal = what;
        return;
    }
    if (kind == kProbe && !l.player1 && size >= kHead + 4u) {
        // Answered on the path it came by: the leading copy times each.
        std::vector<std::uint8_t> out = packet(l, kProbeEcho, kHead + 4u);
        std::memcpy(out.data() + kHead, data + kHead, 4u);
        send_to(l, from, out);
        return;
    }
    if (kind == kProbeEcho && l.player1 && size >= kHead + 4u) {
        const std::uint32_t trip = clock_ms(at) - get<std::uint32_t>(data + kHead);
        if (trip < 10000u) (direct ? l.direct_trips : l.probe_trips).push_back(trip);
        return;
    }
    if (kind == kWelcome && !l.player1 && !l.welcomed && size >= kHead + 18u) {
        const std::size_t length = data[kHead + 16u];
        if (get<std::uint32_t>(data + kHead) != kVersion || size < kHead + 18u + length) return;
        l.delay = get<std::uint32_t>(data + kHead + 4u);
        l.epoch = get<std::uint64_t>(data + kHead + 8u);
        l.answers.assign(reinterpret_cast<const char *>(data + kHead + 17u), length);
        const bool go_direct = data[kHead + 17u + length] == 1u;
        if (const std::optional<About> about = take_about(data + kHead + 18u + length, size - kHead - 18u - length))
            l.their_camera = about->camera;
        if (go_direct && l.direct) {
            l.peer = *l.direct;
            l.on_direct = true;
            l.direct_heard = at;
        } else if (go_direct) {
            // The leading copy heard this one straight, but not the other way
            // round: the relay, which the other copy follows to.
            std::cout << "[" << l.name << "] the other copy chose the direct path, which brought nothing here"
                      << std::endl;
        }
        l.welcomed = true;
        return;
    }
    if (kind == kInput) {
        if (l.on_direct && relayed) move_to_relay(l, "the other copy's input came through the relay");
        take_input(l, data, size, at);
    }
    if (kind == kBye && !l.left) {
        l.left = true;
        l.left_at = Clock::now();
        std::cout << "[" << l.name << "] the other copy left" << std::endl;
    }
}

// Takes in every packet that came, after waiting up to `wait` for the first.
void receive(Link &l, std::chrono::milliseconds wait) {
    if (!l.receiver) return;
    for (const Receiver::Datagram &d : l.receiver->take(wait)) handle(l, d.bytes.data(), d.bytes.size(), d.from, d.at);
}

void close_link_socket(Link &l) {
    l.receiver.reset();  // its thread stops before the socket goes
    if (l.socket != kNoSocket) close_socket(l.socket);
    l.socket = kNoSocket;
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

bool sockets_ready() {
#if defined(_WIN32)
    static const bool started = [] {
        WSADATA wsa{};
        return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
    }();
    return started;
#else
    return true;
#endif
}

bool open_socket(Link &l, const sockaddr_in &bound) {
    if (!sockets_ready()) return false;
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
    l.receiver = std::make_shared<Receiver>(l.socket);
    return true;
}

std::uint32_t env_number(const char *name, std::uint32_t fallback, std::uint32_t most) {
    const char *text = std::getenv(name);
    if (text == nullptr || *text == '\0') return fallback;
    return static_cast<std::uint32_t>(std::min<unsigned long>(std::strtoul(text, nullptr, 10), most));
}

// The input delay for a round trip of `trip` ms: a frame's input goes out
// `delay` frames (of 33 ms) before it is needed, so they must cover half the
// trip and some jitter. With the frames out of balance (Link::slacks) one copy
// has all the slack and the other none, which took a frame more: 2 frames at
// 75 ms waited on nearly every frame; 3 at 100 ms never did
// (docs/port/communication-mode.md, sections 12 to 14).
std::uint32_t delay_for(std::uint32_t trip, bool balanced) {
    constexpr double frame = 1000.0 / 30.0;
    const double frames = (trip / 2.0 + (balanced ? 0.0 : frame) + 15.0) / frame;
    return std::clamp<std::uint32_t>(static_cast<std::uint32_t>(std::ceil(frames)), 2u, 10u);
}

// A wait for the other copy: `turn` (one short turn of it, true while the wait
// goes on) runs as it is for half a second, then on a screen that tells the
// player and lets them leave by holding back. False when they left (or closed
// the window).
bool wait_for(const std::function<bool()> &turn, const char *text = "Waiting for the other player\xE2\x80\xA6 ") {
    const auto began = Clock::now();
    while (Clock::now() - began < kQuietWait)
        if (!turn()) return true;
#if defined(PORTABLEKIT_HAS_RENDERER)
    namespace ui = portablekit::ui;
    const ui::WaitEnd end = ui::wait_over_game([&](ui::WaitView &view) {
        if (!turn()) return false;
        const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - began).count();
        view.text = text + std::to_string(seconds) + " s";
        return true;
    });
    return end != ui::WaitEnd::Left && end != ui::WaitEnd::WindowClosed;
#else
    while (turn()) {
    }
    return true;
#endif
}

// This copy's address on its own network and its link's port, for ASK: the
// address its packets to the relay leave from.
sockaddr_in local_address(const Link &l) {
    sockaddr_in me{};
    AddressSize size = sizeof me;
    if (getsockname(l.socket, reinterpret_cast<sockaddr *>(&me), &size) != 0) return sockaddr_in{};
    if (me.sin_addr.s_addr == htonl(INADDR_ANY) && l.relay) {
        const Socket probe = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (probe != kNoSocket) {
            sockaddr_in via{};
            AddressSize via_size = sizeof via;
            if (::connect(probe, reinterpret_cast<const sockaddr *>(&*l.relay), sizeof *l.relay) == 0 &&
                getsockname(probe, reinterpret_cast<sockaddr *>(&via), &via_size) == 0)
                me.sin_addr = via.sin_addr;
            close_socket(probe);
        }
    }
    return me;
}

// The leading copy, its probing done: the direct path when it answered
// (kDirectLeast times) and is not slower than the relay by more than
// kDirectSlack (two copies on one home Wi-Fi can be slower to each other than
// through a wired server), else the relay. The chosen path's round trips set
// the input delay.
void choose_path(Link &l) {
    if (!l.relay) return;
    const auto median = [](std::vector<std::uint32_t> trips) -> std::optional<std::uint32_t> {
        if (trips.empty()) return std::nullopt;
        std::sort(trips.begin(), trips.end());
        return trips[trips.size() / 2u];
    };
    const auto text = [](std::optional<std::uint32_t> ms) { return ms ? std::to_string(*ms) : std::string("?"); };
    const std::optional<std::uint32_t> direct = median(l.direct_trips), relayed = median(l.probe_trips);
    if (l.direct && l.direct_trips.size() >= kDirectLeast) {
        const std::string trips = "a round trip of " + text(direct) + " ms; through the relay " + text(relayed) + " ms";
        if (relayed && *direct > *relayed + kDirectSlack) {
            std::cout << "[" << l.name << "] the direct path answered but is slower (" << trips
                      << "): the battle goes through the relay" << std::endl;
            return;
        }
        std::cout << "[" << l.name << "] the direct path answered (" << trips << "): the battle goes direct" << std::endl;
        l.peer = *l.direct;
        l.on_direct = true;
        l.direct_heard = Clock::now();
        l.probe_trips = l.direct_trips;
        return;
    }
    if (l.intro_direct)
        std::cout << "[" << l.name << "] no direct path answered" << (l.direct_trips.empty() ? "" : " often enough")
                  << ": the battle goes through the relay" << std::endl;
}

// The listening copy, with the delay on auto, once it has timed a few round
// trips to the joining copy: the delay from their median.
void settle_delay(Link &l) {
    const auto setting_text = [](std::uint32_t fixed) { return fixed == 0u ? std::string("auto") : std::to_string(fixed); };
    const std::string settings =
        " (this copy's setting " + setting_text(l.fixed) + ", the other's " + setting_text(l.their_fixed) + ")";
    if (l.fixed != 0u && l.their_fixed != 0u) {
        l.delay = std::max(l.fixed, l.their_fixed);
        std::cout << "[" << l.name << "] input delay " << l.delay << " frames" << settings << std::endl;
        return;
    }
    std::uint32_t needed = 3u;
    std::string timed = "; no round trip timed";
    if (!l.probe_trips.empty()) {
        std::vector<std::uint32_t> trips = l.probe_trips;
        std::sort(trips.begin(), trips.end());
        const std::uint32_t median = trips[trips.size() / 2u];
        needed = delay_for(median, l.balance);
        l.probe_median = median;
        timed = ", for a round trip of " + std::to_string(median) + " ms (" + std::to_string(trips.size()) +
                " timed, " + std::to_string(trips.front()) + " to " + std::to_string(trips.back()) + ")";
    }
    // With rollback the frames a guess may cover come off the delay the
    // connection needs, down to 1.
    if (const std::uint32_t ahead = battle::rollback_frames(); ahead != 0u && l.name == std::string("battle-net"))
        needed = std::max<std::uint32_t>(1u, needed > ahead ? needed - ahead : 1u);
    l.delay = std::max(l.fixed != 0u ? l.fixed : needed, l.their_fixed != 0u ? l.their_fixed : needed);
    std::cout << "[" << l.name << "] input delay " << l.delay << " frames" << timed << settings << std::endl;
}

// Opens `l` and waits up to D012_NET_WAIT seconds (else `wait_default`) for
// the other copy: as player 1 listening on `address`, else joining it. Player
// 1's copy sets the input delay (D012_NET_DELAY, a number or "auto"), and its
// epoch and answers, which the other takes, and refuses a copy whose
// must-match list differs. On failure `l.refused` or `l.leaving` says why, or
// neither: nobody came.
bool connect(Link &l, bool host, const sockaddr_in &address, const std::string &where, unsigned wait_default,
             bool relayed = false) {
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
    l.balance = std::getenv("D012_NO_NET_BALANCE") == nullptr;
    sockaddr_in bound{};
    bound.sin_family = AF_INET;
    if (relayed) {
        // Both copies send to the session host's relay (Relay), which passes
        // each one's datagrams to the other; the leading copy (host) learns
        // the other's address as the relay's when its HELLO comes through.
        const bool loopback = (ntohl(address.sin_addr.s_addr) >> 24) == 127u;
        bound.sin_addr.s_addr = htonl(loopback ? INADDR_LOOPBACK : INADDR_ANY);
        l.relay = address;
        if (!host) {
            l.peer = address;
            l.peer_known = true;
        }
    } else if (host) {
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
    if (relayed) {
        const std::string always = setting("D012_NET_ALWAYS_RELAY", "online.always_relay");
        l.direct_ok = !(always == "1" || always == "true" || always == "on" || always == "yes");
        l.local = local_address(l);
    }
    const auto wait = std::chrono::seconds(env_number("D012_NET_WAIT", wait_default, 3600u));
    const auto began = Clock::now();
    if (const std::string delay = setting("D012_NET_DELAY", "online.input_delay"); !delay.empty() && delay != "auto")
        l.fixed = static_cast<std::uint32_t>(std::min<unsigned long>(std::strtoul(delay.c_str(), nullptr, 10), 30u));
    if (host) {
        std::cout << "[" << l.name << "] waiting for the other copy on " << where << std::endl;
    } else {
        std::cout << "[" << l.name << "] joining the other copy on " << where << std::endl;
    }
    auto hello = Clock::now() - kHelloEvery;
    auto probed = Clock::time_point{};
    auto punched = Clock::time_point{};
    std::optional<Clock::time_point> probing;
    // The other copy's addresses to send to straight: INTRO's, and the one its
    // packets came from.
    const auto straight = [&] {
        std::vector<sockaddr_in> to;
        if (!l.intro_direct) return to;
        to = l.candidates;
        if (l.direct && std::none_of(to.begin(), to.end(), [&](const sockaddr_in &c) { return same_address(c, *l.direct); }))
            to.push_back(*l.direct);
        return to;
    };
    // Whether a direct path may still come: this copy may go direct and the
    // relay has not said no.
    const auto direct_may = [&] { return l.relay && l.direct_ok && (!l.introduced || l.intro_direct); };
    // One turn of the wait: the joining copy says HELLO until it is welcomed;
    // the listening copy waits for one, then with the delay on auto times
    // round trips (PROBE) for up to kProbeFor and welcomes it. A relayed link
    // asks for an introduction meanwhile, and tries the direct path it gives
    // (the joining copy with PUNCH, the listening one with its PROBEs).
    const auto turn = [&]() -> bool {
        const auto now = Clock::now();
        if (l.refused || now - began >= wait) return false;
        if (l.relay && !l.introduced && now - l.asked >= kHelloEvery) send_ask(l);
        if (!host) {
            if (now - hello >= kHelloEvery) {
                send_hello(l);
                hello = now;
            }
            if (now - punched >= kProbeEvery) {
                for (const sockaddr_in &to : straight()) send_to(l, to, packet(l, kPunch, kHead));
                punched = now;
            }
            receive(l, std::chrono::milliseconds(10));
            return !l.welcomed && !l.refused;
        }
        if (!l.peer_known) {
            if (l.relay && now - hello >= kHelloEvery) {
                send_now(l.socket, *l.relay, packet(l, kHere, kHead));
                hello = now;
            }
            receive(l, std::chrono::milliseconds(10));
            return !l.peer_known || !l.decided;
        }
        if (l.decided) return false;
        if (l.fixed != 0u && l.their_fixed != 0u && !direct_may()) {
            // Both set a number, and one path: nothing to time.
            settle_delay(l);
            l.decided = true;
            send_welcome(l);
            return false;
        }
        if (!probing) probing = now;
        if (now - probed >= kProbeEvery) {
            std::vector<std::uint8_t> out = packet(l, kProbe, kHead + 4u);
            put32(out, kHead, clock_ms());
            send(l, out);
            for (const sockaddr_in &to : straight()) send_to(l, to, out);
            probed = now;
        }
        receive(l, std::chrono::milliseconds(10));
        const bool enough = l.probe_trips.size() >= kProbes && (!direct_may() || l.direct_trips.size() >= kProbes);
        if (!enough && Clock::now() - *probing < kProbeFor) return true;
        choose_path(l);
        settle_delay(l);
        l.decided = true;
        send_welcome(l);
        return false;
    };
    l.leaving = !wait_for(turn);
    // A refused copy keeps saying HELLO for a while: a moment more of
    // answering it, in case the first REFUSEs were lost.
    if (host && l.refused) {
        const auto until = Clock::now() + std::chrono::seconds(1);
        while (Clock::now() < until) receive(l, std::chrono::milliseconds(50));
    }
    if (l.leaving || l.refused || (host ? !l.peer_known || !l.decided : !l.welcomed)) {
        if (l.leaving) std::cout << "[" << l.name << "] this copy's player left while waiting" << std::endl;
        else if (!l.refused) std::cerr << "[" << l.name << "] the other copy did not come" << std::endl;
        close_link_socket(l);
        return false;
    }
    // The joining copy starts its frames when the WELCOME reaches it, half a
    // round trip from now: the listening copy starts with it, so the frames
    // begin in balance (Link::frames) and need not settle into it.
    if (host && l.balance && l.probe_median != 0u)
        std::this_thread::sleep_for(std::chrono::milliseconds(std::min<std::uint32_t>(l.probe_median / 2u, 500u)));
    l.their_count = l.delay;
    l.confirmed = l.delay;
    l.heard = Clock::now();
    l.active = true;
    if (l.on_direct) {
        l.direct_heard = l.heard;
        if (const std::uint32_t cut = env_number("D012_NET_DIRECT_CUT", 0u, 3600u); cut != 0u) {
            l.cut_at = l.heard + std::chrono::seconds(cut);
            std::cout << "[" << l.name << "] the direct path stops in " << cut << " s (D012_NET_DIRECT_CUT)"
                      << std::endl;
        }
    }
    return true;
}

// This frame's share of what puts the two copies' frames level: once a frame.
void hold_frames(Link &l) {
    if (!l.balance) return;
    const std::chrono::microseconds hold = l.frames.hold();
    if (hold.count() <= 0) return;
    portablekit::hle_extension::hold_back(hold);
    l.held += hold;
}

// The link's report, every kReportEvery frames.
void report_frames(Link &l, std::uint64_t frame) {
    if (l.report_from == 0u && frame == 0u) l.report_began = Clock::now();
    if (frame + 1u - l.report_from < kReportEvery) return;
    const auto ms = [](Clock::duration d) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
    };
    const auto now = Clock::now();
    std::cout << "[" << l.name << "] " << l.unit << "s " << l.report_from << "-" << frame << " in "
              << ms(now - l.report_began) << " ms: waited on " << l.waits << ", " << ms(l.waited)
              << " ms in all, the longest " << ms(l.longest) << " ms";
    if (l.frames.known())
        std::cout << "; slack " << static_cast<int>(l.frames.slack()) << " ms here, "
                  << (l.frames.their_slack() == kNoSlack ? std::string("?") : std::to_string(l.frames.their_slack()))
                  << " there; held back " << ms(l.held) << " ms";
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
    l.held = {};
}

// This copy's input for `frame` goes out for frame + delay; returns this
// copy's and the other copy's input for `frame` once the other's is in, or
// the other's as not connected if it is lost.
std::pair<Local, Local> trade(Link &l, std::uint64_t frame, const Local &local) {
    const Clock::time_point needed = Clock::now();
    l.mine[frame + l.delay] = local;
    send_input(l);
    if (frame >= l.delay && !l.lost && l.their_count <= frame) {
        const auto began = Clock::now();
        const auto turn = [&]() -> bool {
            receive(l, std::chrono::milliseconds(2));
            const auto now = Clock::now();
            if (l.on_direct && now - began >= kDirectQuiet && now - l.direct_heard >= kDirectQuiet)
                move_to_relay(l, "nothing came on the direct path for a second");
            if (now - l.sent >= kResend) send_input(l);
            // After a BYE, its last frames may still be on their way (packets
            // can pass each other): a moment for them.
            if ((l.left && now - l.left_at >= kLastFrames) || now - l.heard >= kLost || now - began >= kGiveUp) {
                l.lost = true;
                std::cerr << "[" << l.name << "] lost the other copy at " << l.unit << " " << frame << std::endl;
            }
            return l.their_count <= frame && !l.lost;
        };
        if (!wait_for(turn)) {
            l.lost = true;
            l.leaving = true;
            std::cout << "[" << l.name << "] this copy's player left while waiting at " << l.unit << " " << frame
                      << std::endl;
            const std::vector<std::uint8_t> out = packet(l, kBye, kHead);
            for (int i = 0; i < 3; ++i) send_both(l, out);
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
    if (const auto came = l.arrived.find(frame); came != l.arrived.end() && !l.lost) {
        l.frames.note(needed - came->second);
        hold_frames(l);
    }
    l.arrived.erase(l.arrived.begin(), l.arrived.upper_bound(frame));
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
    report_frames(l, frame);
    return {me, them};
}

// Rollback's trade: this copy's input for `frame` goes out for frame + delay
// (once: a frame run again keeps the input it sent), and both inputs for
// `frame` come back without waiting, the other's guessed as its latest when it
// has not come; `guessed` says so. Waits only while `frame` is `window` or more
// frames past the other's latest input, as the lockstep does.
std::pair<Local, Local> trade_ahead(Link &l, std::uint64_t frame, const Local &local, std::uint32_t window,
                                    bool rerun, bool &guessed) {
    guessed = false;
    if (l.history.count(frame + l.delay) == 0u) {
        l.mine[frame + l.delay] = local;
        l.history[frame + l.delay] = local;
    }
    if (!rerun) send_input(l);
    receive(l, std::chrono::milliseconds(0));
    const Clock::time_point needed = Clock::now();
    if (!rerun && frame >= l.delay && !l.lost && frame >= l.their_count + window) {
        const auto began = Clock::now();
        const auto turn = [&]() -> bool {
            receive(l, std::chrono::milliseconds(2));
            const auto now = Clock::now();
            if (l.on_direct && now - began >= kDirectQuiet && now - l.direct_heard >= kDirectQuiet)
                move_to_relay(l, "nothing came on the direct path for a second");
            if (now - l.sent >= kResend) send_input(l);
            if ((l.left && now - l.left_at >= kLastFrames) || now - l.heard >= kLost || now - began >= kGiveUp) {
                l.lost = true;
                std::cerr << "[" << l.name << "] lost the other copy at " << l.unit << " " << frame << std::endl;
            }
            return frame >= l.their_count + window && !l.lost;
        };
        if (!wait_for(turn)) {
            l.lost = true;
            l.leaving = true;
            const std::vector<std::uint8_t> out = packet(l, kBye, kHead);
            for (int i = 0; i < 3; ++i) send_both(l, out);
        }
        ++l.waits;
        l.waited += Clock::now() - began;
        l.longest = std::max(l.longest, Clock::now() - began);
    }
    if (!rerun) {
        // Most inputs come after their frame has run on the guess: each counts
        // when it comes, or the copy ahead never learns that it is.
        if (frame >= l.delay) l.ran[frame] = needed;
        const std::vector<Clock::duration> slacks = take_slacks(l.ran, l.arrived, frame);
        if (!l.lost && !slacks.empty()) {
            for (const Clock::duration slack : slacks) l.frames.note(slack);
            hold_frames(l);
        }
        report_frames(l, frame);
    }
    const Local me = frame < l.delay ? idle() : l.history.at(frame);
    Local them = idle();
    if (frame >= l.delay) {
        if (const auto at = l.theirs.find(frame); at != l.theirs.end()) {
            them = at->second;
        } else if (l.lost) {
            them.pad.connected = false;
        } else {
            // The guess: their latest input, held.
            if (const auto latest = l.theirs.lower_bound(frame); latest != l.theirs.begin())
                them = std::prev(latest)->second;
            guessed = true;
            l.guessed[frame] = them;
        }
    }
    // What can no longer be run again.
    const std::uint64_t keep = l.their_count > 64u + window ? l.their_count - 64u - window : 0u;
    l.theirs.erase(l.theirs.begin(), l.theirs.lower_bound(keep));
    l.history.erase(l.history.begin(), l.history.lower_bound(keep));
    l.mine.erase(l.mine.begin(), l.mine.lower_bound(std::min(l.confirmed, frame + 1u)));
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
    close_link_socket(l);
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
    if (!connect(l, host, address, text.substr(5u), 300u)) {
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

namespace {

int local_player = 0;  // this copy's fighter in the game: 0 or 1
std::string failure;   // why the last open() failed, for the player
std::string path;      // the way the last battle linked went
// The battle's rollback window (rollback_frames()), fixed as it opens.
std::uint32_t rollback = 0u;
// Through the public server (Play online), rollback by default: its window,
// the one the Deck's tests used (communication-mode.md, section 19.6;
// online-matchmaking.md, section 14).
constexpr std::uint32_t kServerRollback = 7u;

// The window for a battle as it opens: the setting (read once), else
// kServerRollback through the public server and none otherwise.
std::uint32_t rollback_for(bool through_public_server) {
    static const std::string text = setting("D012_NET_ROLLBACK", "online.rollback");
    if (text.empty() || text == "auto") return through_public_server ? kServerRollback : 0u;
    if (text == "off" || text == "0") return 0u;
    return static_cast<std::uint32_t>(std::clamp<unsigned long>(std::strtoul(text.c_str(), nullptr, 10), 1u, 12u));
}

// Rollback's leading copy: the frames below `upto` whose inputs are both
// certain (the other's in, below their_count) go into the watchers' stream,
// with the inputs the frames run again used, not the guesses.
void stream_certain(Link &l, std::uint64_t upto) {
    if (!l.player1 || !l.relay || l.lost) return;
    while (l.stream.size() < std::min(upto, l.their_count)) {
        const std::uint64_t frame = l.stream.size();
        Local me = idle(), them = idle();
        if (frame >= l.delay) {
            if (const auto at = l.history.find(frame); at != l.history.end()) me = at->second;
            if (const auto at = l.theirs.find(frame); at != l.theirs.end()) them = at->second;
        }
        l.stream.emplace_back(me.pad, them.pad);  // the leading copy is player 0
    }
    send_stream(l);
}

// A watching copy's end of the stream (watch()).
struct Watch {
    bool active = false;
    std::uint32_t tag = 0;
    std::uint32_t id = 0;
    Socket socket = kNoSocket;
    std::shared_ptr<Receiver> receiver;
    sockaddr_in relay{};
    std::vector<std::pair<input::Pad, input::Pad>> frames;  // from the battle's first, as they came
    std::map<std::uint64_t, std::uint32_t> mine, theirs;    // check values not yet compared
    Camera cameras[2];
    std::optional<std::uint64_t> end;  // the leader's battle ended there
    bool heard_any = false;
    bool differs = false;  // the leader's must-match list is not this copy's
    bool apart = false;
    bool lost = false;
    bool leaving = false;
    std::uint64_t checks = 0;
    Clock::time_point began{}, heard{}, said{};
    // The buffer (kWatchStart): the frames wanted in hand, fixed by
    // D012_WATCH_BUFFER=N; D012_WATCH_SKIP=0 never skips.
    std::uint64_t buffer = kWatchStart;
    bool adapts = true;
    bool skips = true;
    std::uint64_t leader_holds = 0;  // as the newest STREAM said
    bool waiting = false;
    bool leader_dry = false;  // a STREAM in this wait: the leader had no frame more
    std::uint64_t skip_until = 0;  // frames before it run unseen
    std::uint64_t look_from = 0;   // the frames in hand since it, for a skip
    std::uint64_t look_least = ~std::uint64_t{0};
    // When the last frame was handed out, for a slow moment of this copy's.
    Clock::time_point left{};
    bool left_any = false;
    // The most in hand in this block of kWatchDriftLook frames and the last.
    std::uint64_t drift_from = 0, drift_most = 0, drift_last = 0;
    // The window the buffer may be made smaller after.
    std::uint64_t window_from = 0;
    std::uint64_t window_least = ~std::uint64_t{0};
    bool window_waited = false;
    // Since the last report.
    std::uint64_t report_from = 0;
    Clock::time_point report_began{};
    std::uint64_t waits = 0, stalls = 0, skips_made = 0, skipped = 0;
    Clock::duration waited{}, held{};
    // D012_WATCH_PAUSE=FRAME:MS[,FRAME:MS...], a test switch: this copy
    // stops for MS at FRAME, as a slow moment of its own would.
    std::map<std::uint64_t, long> pauses;
};

Watch &watch_state() {
    static Watch w;
    return w;
}

void compare_watch_checks(Watch &w) {
    for (auto it = w.mine.begin(); it != w.mine.end();) {
        const auto theirs = w.theirs.find(it->first);
        if (theirs == w.theirs.end()) {
            ++it;
            continue;
        }
        if (theirs->second != it->second && !w.apart) {
            w.apart = true;
            std::cerr << "[battle-watch] this copy's battle differs from the fighters' at frame " << it->first
                      << std::endl;
        } else if (!w.apart && (w.checks++ % 64u) == 0u) {
            std::cout << "[battle-watch] in step with the fighters at frame " << it->first << std::endl;
        }
        w.theirs.erase(theirs);
        it = w.mine.erase(it);
    }
    while (w.mine.size() > 256u) w.mine.erase(w.mine.begin());
    while (w.theirs.size() > 256u) w.theirs.erase(w.theirs.begin());
}

void take_stream(Watch &w, const std::uint8_t *data, std::size_t size) {
    if (size < kStreamHead) return;
    const std::uint64_t first = get<std::uint64_t>(data + kHead);
    const std::uint64_t count = get<std::uint16_t>(data + kHead + 8u);
    const std::size_t check_count = data[kHead + 11u];
    if (size < kStreamHead + check_count * 8u + count * 2u * kLocalSize) return;
    w.heard = Clock::now();
    if (!w.heard_any) {
        w.heard_any = true;
        std::cout << "[battle-watch] the fighters' stream came" << std::endl;
    }
    if (get<std::uint32_t>(data + kHead + 16u) != matched_hash() && !w.differs) {
        w.differs = true;
        std::cerr << "[battle-watch] the fighters' games differ from this one (the port's version, mods or settings)"
                  << std::endl;
    }
    if (data[kHead + 10u] != 0u && !w.end) {
        w.end = get<std::uint32_t>(data + kHead + 12u);
        std::cout << "[battle-watch] the fighters' battle ended at frame " << *w.end << std::endl;
    }
    w.leader_holds = std::max<std::uint64_t>(w.leader_holds, get<std::uint32_t>(data + kHead + 12u));
    w.cameras[0] = take_camera(data + kHead + 20u);
    w.cameras[1] = take_camera(data + kHead + 29u);
    std::size_t at = kStreamHead;
    for (std::size_t i = 0; i < check_count; ++i, at += 8u)
        w.theirs[get<std::uint32_t>(data + at)] = get<std::uint32_t>(data + at + 4u);
    for (std::uint64_t i = 0; i < count; ++i, at += 2u * kLocalSize) {
        if (first + i != w.frames.size()) continue;  // held already, or after a gap
        w.frames.emplace_back(take_pad(data + at), take_pad(data + at + kLocalSize));
    }
    // Waiting, and the leader had no frame this copy lacks: the fighters
    // stalled, not the path.
    if (w.waiting && w.leader_holds <= w.frames.size()) w.leader_dry = true;
    compare_watch_checks(w);
}

void say_watching(Watch &w) {
    std::vector<std::uint8_t> out(kHead + 12u, 0u);
    std::memcpy(out.data(), kMagic, 4u);
    out[4] = kWatch;
    out[5] = static_cast<std::uint8_t>(w.tag & 0xFFu);
    out[6] = static_cast<std::uint8_t>((w.tag >> 8u) & 0xFFu);
    out[7] = static_cast<std::uint8_t>((w.tag >> 16u) & 0xFFu);
    put32(out, kHead, w.id);
    put64(out, kHead + 4u, w.frames.size());
    Simulated &sim = Simulated::get();
    if (sim.on()) sim.send(w.socket, w.relay, out);
    else send_now(w.socket, w.relay, out);
    w.said = Clock::now();
}

void watch_receive(Watch &w, std::chrono::milliseconds wait) {
    if (!w.receiver) return;
    for (const Receiver::Datagram &d : w.receiver->take(wait)) {
        if (d.bytes.size() < kHead || std::memcmp(d.bytes.data(), kMagic, 4u) != 0 || tag_of(d.bytes.data()) != w.tag ||
            d.bytes[4] != kStream || !same_address(d.from, w.relay))
            continue;
        take_stream(w, d.bytes.data(), d.bytes.size());
    }
    if (Clock::now() - w.said >= kWatchEvery) say_watching(w);
}

bool on(const std::string &value) { return value == "1" || value == "true" || value == "on" || value == "yes"; }

// Without online.battle_link: both copies go through the relay of the copy
// hosting the session, at the battle port of the session's address (the
// loopback one on the host itself). The game's player 0 leads the link.
bool from_session(sockaddr_in &address, std::string &where) {
    const std::string port_text = setting("D012_NET_BATTLE_PORT", "online.battle_port");
    const long number = port_text.empty() ? kBattlePort : std::strtol(port_text.c_str(), nullptr, 10);
    if (number <= 0 || number > 65535) return false;
    const auto port = static_cast<std::uint16_t>(number);
    std::string host = portablekit::adhoc_server_address();
    if (const std::size_t colon = host.rfind(':'); colon != std::string::npos && host.find(':') == colon)
        host.resize(colon);
    if (host.empty() || !sockets_ready()) return false;
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    addrinfo *found = nullptr;
    if (getaddrinfo(host.c_str(), nullptr, &hints, &found) != 0 || found == nullptr) return false;
    const in_addr server = reinterpret_cast<const sockaddr_in *>(found->ai_addr)->sin_addr;
    freeaddrinfo(found);
    const bool hosting = portablekit::adhoc_hosting();
    address = sockaddr_in{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = hosting ? htonl(INADDR_LOOPBACK) : server.s_addr;
    where = (hosting ? std::string("this copy's relay") : "the relay at " + host) + ", UDP port " + std::to_string(port);
    return true;
}

} // namespace

bool configured() {
    return !setting("D012_NET_BATTLE", "online.battle_link").empty() || on(setting("D012_ADHOC", "network.adhoc"));
}

std::uint16_t port() {
    const std::string text = setting("D012_NET_BATTLE_PORT", "online.battle_port");
    const long number = text.empty() ? kBattlePort : std::strtol(text.c_str(), nullptr, 10);
    return number > 0 && number <= 65535 ? static_cast<std::uint16_t>(number) : kBattlePort;
}

// The session host's relay for battles (d012_battle_relay.hpp), open while
// this copy hosts the session. So the two fighters need not be the host, and
// only the host's router needs the port open. The host's own fighter reaches
// it on loopback. It answers WHERE as the public server does, from the
// session's players (d012_where.hpp), kept up to date twice a second as the
// relay asks whether to stay open; both on the relay's thread.
void serve() {
    if (!setting("D012_NET_BATTLE", "online.battle_link").empty()) return;
    static d012::where::Tally tally;
    relay::Options options;
    options.port = port();
    // D012_ADHOC_LOOPBACK: hosting opens nothing beyond this computer.
    options.loopback_only = portablekit::adhoc_loopback_only();
    options.open = [] {
        const bool hosting = portablekit::adhoc_hosting();
        std::vector<d012::where::Player> players;
        if (hosting)
            for (const auto &player : portablekit::adhoc_host_status().players) {
                std::uint64_t mac = 0u;
                for (const std::uint8_t byte : player.mac) mac = mac << 8u | byte;
                players.push_back({mac, player.version, player.group});
            }
        tally.update(players);
        return hosting;
    };
    options.where = [](std::string_view version) { return tally.of(version); };
    relay::start(std::move(options));
}

void must_match(const std::string &what, const std::string &value) {
    auto &items = matched();
    const auto found = std::find_if(items.begin(), items.end(), [&](const auto &item) { return item.first == what; });
    if (found != items.end()) found->second = value;
    else items.emplace_back(what, value);
}

std::vector<std::pair<std::string, std::string>> must_match_items() { return matched(); }

void set_camera(const Camera &mine) { my_camera() = mine; }

Camera camera(int player) {
    if (watch_state().active) return watch_state().cameras[player == 0 ? 0 : 1];
    return player == local_player ? my_camera() : battle_link().their_camera;
}

std::string why_not() { return failure; }

void notice(const std::string &text) {
    std::cout << "[battle-net] told the player: " << text << std::endl;
#if defined(PORTABLEKIT_HAS_RENDERER)
    namespace ui = portablekit::ui;
    ui::wait_over_game([&](ui::WaitView &view) {
        view.text = text;
        view.ok = true;
        view.can_leave = false;
        return true;
    });
#endif
}

bool open(int player, std::uint32_t tag) {
    Link &l = battle_link();
    shut(l);
    failure.clear();
    local_player = player == 0 ? 0 : 1;
    const std::string text = setting("D012_NET_BATTLE", "online.battle_link");
    sockaddr_in address{};
    bool listen = player == 0;
    std::string where = text;
    if (!text.empty() && !parse_address(text, address)) {
        std::cerr << "[battle-net] D012_NET_BATTLE is PORT or ADDRESS:PORT, not \"" << text << "\"" << std::endl;
        return false;
    }
    if (text.empty() && !from_session(address, where)) {
        std::cerr << "[battle-net] no session address to link the battle through" << std::endl;
        return false;
    }
    l.name = "battle-net";
    l.unit = "frame";
    l.tag = tag & kTagMask;
    // Before the link: the leading copy's automatic delay counts the window.
    rollback = rollback_for(text.empty() && portablekit::adhoc_online() && !portablekit::adhoc_hosting());
    if (rollback != 0u) std::cout << "[battle-net] rollback: up to " << rollback << " frames ahead" << std::endl;
    if (!connect(l, listen, address, where, 30u, text.empty())) {
        if (l.refused) {
            failure = "This battle cannot start: the other player's game differs in " + l.refusal +
                      ". Both players need the same version of the port, the same mods that change the game, and "
                      "the same settings.";
            // What a player can do about the likeliest difference.
            if (l.refusal.find("the port's version") != std::string::npos)
                failure += " To see whether a newer version is out, choose Check for updates on the menu's "
                           "System page.";
        }
        else if (!l.leaving && listen && !text.empty())
            failure = "The other player's game did not connect for this battle.";
        else if (!l.leaving)
            failure = "The other player's game did not answer. If this happens every time, the host's router may "
                      "not be passing the battle's UDP port " +
                      std::to_string(ntohs(address.sin_port)) + " (see Playing online in the README).";
        return false;
    }
    path = text.empty() ? (l.on_direct ? "direct" : "through the relay") : "the battle link's address";
    std::cout << "[battle-net] linked as player " << player << " for battle " << l.tag << " ("
              << (text.empty() ? path + ", " : std::string()) << (listen ? "leading" : "joined") << "): " << l.delay
              << " frames of input delay" << std::endl;
    return true;
}

std::string last_path() {
    const Link &l = battle_link();
    if (l.active && l.relay) return l.on_direct ? "direct" : "through the relay";
    return path;
}

bool open_now() { return battle_link().active; }

Pads exchange(std::uint64_t frame, const input::Pad &mine) {
    Link &l = battle_link();
    Local local;
    local.pad = mine;
    const auto [me, them] = trade(l, frame, local);
    Pads pads;
    pads.player[local_player] = me.pad;
    pads.player[1 - local_player] = them.pad;
    pads.lost = l.lost;
    pads.left = l.leaving;
    // The lockstep's frames are certain as they come back.
    if (l.player1 && l.relay && !l.lost && frame == l.stream.size()) {
        l.stream.emplace_back(pads.player[0], pads.player[1]);
        send_stream(l);
    }
    return pads;
}

std::uint32_t rollback_frames() { return rollback; }

Pads exchange_ahead(std::uint64_t frame, const input::Pad &mine, bool rerun, bool &guessed) {
    Link &l = battle_link();
    Local local;
    local.pad = mine;
    const auto [me, them] = trade_ahead(l, frame, local, rollback_frames(), rerun, guessed);
    Pads pads;
    pads.player[local_player] = me.pad;
    pads.player[1 - local_player] = them.pad;
    pads.lost = l.lost;
    pads.left = l.leaving;
    if (!rerun) stream_certain(l, frame + 1u);
    return pads;
}

std::optional<std::uint64_t> take_wrong_guess() {
    Link &l = battle_link();
    receive(l, std::chrono::milliseconds(0));
    const std::optional<std::uint64_t> wrong = l.wrong_from;
    l.wrong_from.reset();
    return wrong;
}

std::uint64_t confirmed() { return battle_link().their_count; }

bool check(std::uint64_t frame, std::uint64_t hash) {
    Link &l = battle_link();
    note_hash(l, frame, hash);
    // Only certain frames come here (rollback compares no other), so the
    // watchers may compare them too.
    if (l.player1 && l.relay) {
        l.stream_checks[frame] = static_cast<std::uint32_t>(hash);
        while (l.stream_checks.size() > 64u) l.stream_checks.erase(l.stream_checks.begin());
    }
    return !l.apart;
}

void close(std::uint64_t reached) {
    Link &l = battle_link();
    if (l.active && !l.lost && !l.left) {
        // A battle that ends normally ends on the same frame on both copies,
        // and the other may still want this copy's last frames: they go again
        // first. Each is said a few times, since a datagram may be lost.
        for (int i = 0; i < 3; ++i) send_input(l);
        const std::vector<std::uint8_t> out = packet(l, kBye, kHead);
        for (int i = 0; i < 3; ++i) send_both(l, out);
    }
    if (l.active && l.player1 && l.relay && !l.watchers.empty()) {
        // The watchers: every frame of the battle, and where it ended, until
        // each holds them all (or kStreamFlush). With rollback the other's
        // last inputs may still be on their way (it sends them as it closes).
        // A watcher's battle ends where the stream does: a battle that ends by
        // itself does so on that frame on every copy, and one that was left
        // ends there with the game's error.
        const auto began = Clock::now();
        for (;;) {
            receive(l, std::chrono::milliseconds(10));
            if (rollback_frames() != 0u) stream_certain(l, reached);
            send_stream(l, l.stream.size());
            const auto now = Clock::now();
            const bool all = std::all_of(l.watchers.begin(), l.watchers.end(), [&](const auto &w) {
                return w.second.holds >= l.stream.size() || now - w.second.heard > kLost;
            });
            if ((all && l.stream.size() >= reached) || now - began >= kStreamFlush) break;
        }
        if (!l.watchers.empty())
            std::cout << "[" << l.name << "] the battle's " << l.stream.size() << " frames streamed to "
                      << l.watchers.size() << (l.watchers.size() == 1u ? " watcher" : " watchers") << std::endl;
    }
    shut(l);
}

bool can_stream() { return setting("D012_NET_BATTLE", "online.battle_link").empty(); }

bool watch(std::uint32_t tag) {
    Watch &w = watch_state();
    unwatch();
    failure.clear();
    sockaddr_in relay{};
    std::string where;
    if (!from_session(relay, where)) {
        std::cerr << "[battle-watch] no session address to watch the battle through" << std::endl;
        return false;
    }
    w = Watch{};
    w.tag = tag & kTagMask;
    w.relay = relay;
    w.id = static_cast<std::uint32_t>(std::random_device{}()) ^
           static_cast<std::uint32_t>(Clock::now().time_since_epoch().count());
    if (!sockets_ready()) return false;
    w.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (w.socket == kNoSocket) return false;
#if defined(_WIN32)
    u_long on = 1u;
    ioctlsocket(w.socket, FIONBIO, &on);
    constexpr DWORD kUdpConnReset = 0x9800000Cu;  // SIO_UDP_CONNRESET
    BOOL report = FALSE;
    DWORD bytes = 0;
    WSAIoctl(w.socket, kUdpConnReset, &report, sizeof report, nullptr, 0, &bytes, nullptr, nullptr);
#else
    fcntl(w.socket, F_SETFL, fcntl(w.socket, F_GETFL, 0) | O_NONBLOCK);
#endif
    sockaddr_in bound{};
    bound.sin_family = AF_INET;
    const bool loopback = (ntohl(relay.sin_addr.s_addr) >> 24) == 127u;
    bound.sin_addr.s_addr = htonl(loopback ? INADDR_LOOPBACK : INADDR_ANY);
    if (bind(w.socket, reinterpret_cast<const sockaddr *>(&bound), sizeof bound) != 0) {
        close_socket(w.socket);
        w.socket = kNoSocket;
        return false;
    }
    w.receiver = std::make_shared<Receiver>(w.socket);
    w.active = true;
    w.began = w.heard = Clock::now();
    if (const char *text = std::getenv("D012_WATCH_BUFFER"); text != nullptr && *text != '\0') {
        const long frames = std::strtol(text, nullptr, 10);
        if (frames >= 1 && frames <= 600) {
            w.buffer = static_cast<std::uint64_t>(frames);
            w.adapts = false;
        }
    }
    if (const char *text = std::getenv("D012_WATCH_SKIP"); text != nullptr && std::string(text) == "0") w.skips = false;
    if (const char *text = std::getenv("D012_WATCH_PAUSE"); text != nullptr) {
        for (const char *at = text; *at != '\0';) {
            char *end = nullptr;
            const unsigned long long frame = std::strtoull(at, &end, 10);
            if (end == at || *end != ':') break;
            at = end + 1;
            const long pause = std::strtol(at, &end, 10);
            if (end == at) break;
            w.pauses[frame] = pause;
            at = *end == ',' ? end + 1 : end;
        }
    }
    say_watching(w);
    std::cout << "[battle-watch] watching battle " << w.tag << " through " << where << std::endl;
    return true;
}

bool watching() { return watch_state().active; }

Pads watch_frame(std::uint64_t frame) {
    Watch &w = watch_state();
    Pads pads;
    if (const auto pause = w.pauses.find(frame); pause != w.pauses.end()) {
        std::cout << "[battle-watch] stops for " << pause->second << " ms at frame " << frame << " (D012_WATCH_PAUSE)"
                  << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(pause->second));
    }
    // This copy was slow since the last frame (not waiting for one: that is
    // inside this call).
    const bool slow = w.left_any && Clock::now() - w.left > kWatchSlow;
    watch_receive(w, std::chrono::milliseconds(0));
    const auto ms = [](Clock::duration d) { return std::chrono::duration_cast<std::chrono::milliseconds>(d).count(); };
    const auto ended = [&] { return w.end && w.frames.size() >= *w.end; };
    // The first frame, or one that has not come: wait until the buffer is in
    // too, so the frames after it do not each wait a little.
    if ((frame == 0u || frame >= w.frames.size()) && !ended() && !w.lost && !w.differs) {
        const auto began = Clock::now();
        std::uint64_t want = frame + 1u + w.buffer;
        bool came = false;  // the frame itself
        w.waiting = true;
        w.leader_dry = false;
        const auto turn = [&]() -> bool {
            watch_receive(w, std::chrono::milliseconds(2));
            const auto now = Clock::now();
            if ((w.heard_any && now - w.heard >= kLost) || (!w.heard_any && now - w.began >= kWatchFirst)) {
                w.lost = true;
                std::cerr << "[battle-watch] the fighters' stream stopped at frame " << frame << std::endl;
            }
            if (!came && w.frames.size() > frame) {
                came = true;
                // The path made this copy wait: a larger buffer. Not at the
                // start (the fighters link then), nor for the fighters' stall.
                if (frame != 0u && !w.leader_dry && w.adapts) {
                    const auto frames = static_cast<std::uint64_t>((now - began) / kWatchFrameTime);
                    const std::uint64_t larger = std::min(kWatchMost, w.buffer + kWatchRaise + frames);
                    if (larger != w.buffer)
                        std::cout << "[battle-watch] waited " << ms(now - began) << " ms at frame " << frame
                                  << ": a buffer of " << larger << " frames (was " << w.buffer << ")" << std::endl;
                    w.buffer = larger;
                    want = frame + 1u + w.buffer;
                    // The frames in hand before it say nothing of the pace.
                    w.drift_most = w.drift_last = w.buffer;
                }
                if (frame != 0u && !w.leader_dry) w.window_waited = true;
                if (frame != 0u && w.leader_dry) ++w.stalls;
            }
            return !ended() && w.frames.size() < want && !w.lost && !w.differs;
        };
        if (!wait_for(turn, "Waiting for the fighters\xE2\x80\xA6 ")) {
            w.lost = true;
            w.leaving = true;
            std::cout << "[battle-watch] this copy's player left while waiting at frame " << frame << std::endl;
        }
        w.waiting = false;
        if (frame != 0u) {
            ++w.waits;
            w.waited += Clock::now() - began;
        }
    }
    if (w.differs && failure.empty())
        failure = "This battle cannot be watched: this game differs from the fighters' games. Everyone needs the "
                  "same version of the port, the same mods that change the game, and the same settings.";
    if (frame >= w.frames.size() || w.differs) {
        // The leader's battle ended before this frame, or it cannot be had.
        if (!w.lost && w.end) std::cout << "[battle-watch] the fighters' battle was left at frame " << *w.end << std::endl;
        pads.lost = true;
        pads.left = w.leaving;
        return pads;
    }
    pads.player[0] = w.frames[frame].first;
    pads.player[1] = w.frames[frame].second;
    const std::uint64_t in_hand = w.frames.size() - frame - 1u;
    // Well behind the buffer for kWatchLook frames on end (not a moment's
    // bunch of late packets): up to it unseen, once the battle goes on. A
    // skip just ended goes on if as many more came while it ran.
    if (frame >= w.skip_until) w.look_least = std::min(w.look_least, in_hand);
    const bool skip_ended = frame != 0u && frame == w.skip_until;
    const std::uint64_t skip_at = w.buffer + std::max(kWatchSkip, w.buffer / 4u);
    std::uint64_t over = 0u;
    if ((skip_ended || (slow && frame >= w.skip_until)) && in_hand >= skip_at) {
        over = in_hand - w.buffer;
    } else if (frame >= w.skip_until && frame + 1u - w.look_from >= kWatchLook) {
        if (w.look_least != ~std::uint64_t{0} && w.look_least >= skip_at) over = w.look_least - w.buffer;
        w.look_from = frame + 1u;
        w.look_least = ~std::uint64_t{0};
    }
    if (w.skips && !w.end && over != 0u) {
        w.skip_until = frame + over;
        w.look_from = w.skip_until;
        w.look_least = ~std::uint64_t{0};
        if (!skip_ended) ++w.skips_made;
        w.skipped += w.skip_until - frame;
        std::cout << "[battle-watch] " << in_hand << " frames in hand at frame " << frame << ": runs "
                  << (w.skip_until - frame) << " unseen to the buffer of " << w.buffer
                  << (skip_ended ? " (more came as it skipped)" : "") << std::endl;
    }
    pads.unseen = frame < w.skip_until;
    // Following the fighters' pace.
    if (frame + 1u - w.drift_from >= kWatchDriftLook) {
        w.drift_last = w.drift_most;
        w.drift_most = 0u;
        w.drift_from = frame + 1u;
    }
    w.drift_most = std::max(w.drift_most, in_hand);
    if (!w.end && !pads.unseen && frame >= kWatchDriftLook &&
        std::max(w.drift_most, w.drift_last) + kWatchDrift < w.buffer) {
        portablekit::hle_extension::hold_back(kWatchHold);
        w.held += kWatchHold;
    }
    // The buffer smaller, after a window in which the path never made this
    // copy wait and more than it needed was always in hand.
    if (!pads.unseen) w.window_least = std::min(w.window_least, in_hand);
    if (frame + 1u - w.window_from >= kWatchWindow) {
        const std::uint64_t margin = std::max(kWatchMargin, w.buffer / 4u);
        if (w.adapts && !w.window_waited && w.window_least != ~std::uint64_t{0} && w.window_least > margin &&
            w.buffer > kWatchLeast) {
            const std::uint64_t smaller =
                std::max(kWatchLeast, w.buffer - std::min(kWatchLower, w.window_least - margin));
            std::cout << "[battle-watch] at least " << w.window_least << " frames in hand since frame "
                      << w.window_from << ": a buffer of " << smaller << " frames (was " << w.buffer << ")"
                      << std::endl;
            w.buffer = smaller;
        }
        w.window_from = frame + 1u;
        w.window_least = ~std::uint64_t{0};
        w.window_waited = false;
    }
    if (w.report_from == 0u && frame == 0u) w.report_began = Clock::now();
    if (frame + 1u - w.report_from >= kReportEvery) {
        const auto now = Clock::now();
        std::cout << "[battle-watch] frames " << w.report_from << "-" << frame << " in " << ms(now - w.report_began)
                  << " ms: waited on " << w.waits << " (" << w.stalls << " the fighters' stalls), " << ms(w.waited)
                  << " ms in all; held back " << ms(w.held) << " ms; skipped " << w.skipped << " frames in "
                  << w.skips_made << "; buffer " << w.buffer << ", " << in_hand << " frames in hand" << std::endl;
        w.report_began = now;
        w.report_from = frame + 1u;
        w.waits = 0u;
        w.stalls = 0u;
        w.skips_made = 0u;
        w.skipped = 0u;
        w.waited = {};
        w.held = {};
    }
    w.left = Clock::now();
    w.left_any = true;
    return pads;
}

bool watch_check(std::uint64_t frame, std::uint32_t value) {
    Watch &w = watch_state();
    w.mine[frame] = value;
    compare_watch_checks(w);
    return !w.apart;
}

void unwatch() {
    Watch &w = watch_state();
    if (!w.active) return;
    w.receiver.reset();  // its thread stops before the socket goes
    if (w.socket != kNoSocket) close_socket(w.socket);
    w.socket = kNoSocket;
    w.active = false;
    std::cout << "[battle-watch] stopped watching at frame " << w.frames.size() << " of the stream" << std::endl;
}

} // namespace battle

} // namespace d012::net
