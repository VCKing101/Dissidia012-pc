// The battle relay (d012_battle_relay.hpp).
//
// MIT licence; see port/README.md.

#if defined(_WIN32) && !defined(NOMINMAX)
#define NOMINMAX  // <windows.h>'s min and max macros break std::min under MSVC
#endif

#include "d012_battle_relay.hpp"

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
#include <cstring>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

namespace d012::net::relay {
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

constexpr std::size_t kMostWatchers = 16u;  // the game has at most 8
constexpr std::size_t kLongestTag = 48u;    // as PortableKit's version tags

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

bool same_address(const sockaddr_in &a, const sockaddr_in &b) {
    return a.sin_addr.s_addr == b.sin_addr.s_addr && a.sin_port == b.sin_port;
}

std::uint32_t tag_of(const std::uint8_t *data) {
    return static_cast<std::uint32_t>(data[5]) | (static_cast<std::uint32_t>(data[6]) << 8u) |
           (static_cast<std::uint32_t>(data[7]) << 16u);
}

void put32(std::string &out, std::uint32_t value) {
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<char>((value >> (8 * i)) & 0xFFu));
}

std::uint32_t get32(const std::uint8_t *data) {
    return static_cast<std::uint32_t>(data[0]) | (static_cast<std::uint32_t>(data[1]) << 8u) |
           (static_cast<std::uint32_t>(data[2]) << 16u) | (static_cast<std::uint32_t>(data[3]) << 24u);
}

std::string head(std::uint8_t kind, std::uint32_t tag = 0u) {
    std::string out(kMagic, 4u);
    out.push_back(static_cast<char>(kind));
    for (int i = 0; i < 3; ++i) out.push_back(static_cast<char>((tag >> (8 * i)) & 0xFFu));
    return out;
}

// An endpoint's six bytes, as they are in a sockaddr_in (network order).
void put_endpoint(std::string &out, Endpoint e) {
    out.append(reinterpret_cast<const char *>(&e.address), 4u);
    out.append(reinterpret_cast<const char *>(&e.port), 2u);
}

Endpoint get_endpoint(const std::uint8_t *data) {
    Endpoint e;
    std::memcpy(&e.address, data, 4u);
    std::memcpy(&e.port, data + 4u, 2u);
    return e;
}

Endpoint endpoint_of(const sockaddr_in &a) { return Endpoint{a.sin_addr.s_addr, a.sin_port}; }

Socket open_socket(std::uint16_t port, bool loopback_only, const std::string &name) {
    if (!sockets_ready()) return kNoSocket;
    const Socket socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket == kNoSocket) return kNoSocket;
#if defined(_WIN32)
    constexpr DWORD kUdpConnReset = 0x9800000Cu;  // SIO_UDP_CONNRESET: a copy gone must not fail the next read
    BOOL report = FALSE;
    DWORD bytes = 0;
    WSAIoctl(socket, kUdpConnReset, &report, sizeof report, nullptr, 0, &bytes, nullptr, nullptr);
    u_long on = 1u;
    ioctlsocket(socket, FIONBIO, &on);
#else
    fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) | O_NONBLOCK);
#endif
    sockaddr_in bound{};
    bound.sin_family = AF_INET;
    bound.sin_port = htons(port);
    bound.sin_addr.s_addr = htonl(loopback_only ? INADDR_LOOPBACK : INADDR_ANY);
    if (bind(socket, reinterpret_cast<const sockaddr *>(&bound), sizeof bound) != 0) {
        close_socket(socket);
        std::cerr << "[" << name << "] cannot open UDP port " << port << "; battles through it fail" << std::endl;
        return kNoSocket;
    }
    std::cout << "[" << name << "] relaying battles on UDP port " << port << (loopback_only ? " (127.0.0.1)" : "")
              << std::endl;
    return socket;
}

struct End {
    End(const sockaddr_in &at, Clock::time_point when) : address(at), heard(when) {}
    sockaddr_in address;
    Clock::time_point heard;
    // Its ASK, once one came: whether it may go direct, and its address on its
    // own network. `told`: it has had an INTRO.
    bool asked = false;
    bool direct = false;
    Endpoint local;
    bool told = false;
};

// The end at `from` among a battle's two: known, added, or put in the place of
// one quiet for 5 s (a copy that came back on a new port). Null for a third
// address while both are heard: it is not this battle's.
End *place(std::vector<End> &list, const sockaddr_in &from) {
    const auto now = Clock::now();
    const auto me =
        std::find_if(list.begin(), list.end(), [&](const End &e) { return same_address(e.address, from); });
    if (me != list.end()) {
        me->heard = now;
        return &*me;
    }
    if (list.size() < 2u) {
        list.push_back(End{from, now});
        return &list.back();
    }
    const auto quiet =
        std::min_element(list.begin(), list.end(), [](const End &a, const End &b) { return a.heard < b.heard; });
    if (now - quiet->heard < std::chrono::seconds(5)) return nullptr;
    *quiet = End{from, now};
    return &*quiet;
}

void run(const Options &options) {
    Socket socket = kNoSocket;
    auto checked = Clock::time_point{};
    std::map<std::uint32_t, std::vector<End>> ends;      // by tag, at most two
    std::map<std::uint32_t, std::vector<End>> watchers;  // by tag, the copies watching that battle
    const auto forget = [](std::map<std::uint32_t, std::vector<End>> &by_tag, Clock::time_point now,
                           Clock::duration quiet) {
        for (auto it = by_tag.begin(); it != by_tag.end();) {
            auto &list = it->second;
            list.erase(std::remove_if(list.begin(), list.end(), [&](const End &e) { return now - e.heard > quiet; }),
                       list.end());
            it = list.empty() ? by_tag.erase(it) : std::next(it);
        }
    };
    for (;;) {
        const auto now = Clock::now();
        if (now - checked >= std::chrono::milliseconds(500)) {
            checked = now;
            const bool wanted = !options.open || options.open();
            if (wanted && socket == kNoSocket) socket = open_socket(options.port, options.loopback_only, options.name);
            if (!wanted && socket != kNoSocket) {
                close_socket(socket);
                socket = kNoSocket;
                ends.clear();
                watchers.clear();
                std::cout << "[" << options.name << "] closed" << std::endl;
            }
            forget(ends, now, std::chrono::seconds(60));
            forget(watchers, now, std::chrono::seconds(10));
        }
        if (socket == kNoSocket) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            continue;
        }
        fd_set set;
        FD_ZERO(&set);
        FD_SET(socket, &set);
        timeval time{0, 50000};
        if (select(static_cast<int>(socket + 1), &set, nullptr, nullptr, &time) <= 0) continue;
        for (;;) {
            std::uint8_t data[2048];
            sockaddr_in from{};
            AddressSize from_size = sizeof from;
            const int got = static_cast<int>(recvfrom(socket, reinterpret_cast<char *>(data), sizeof data, 0,
                                                      reinterpret_cast<sockaddr *>(&from), &from_size));
            if (got < 0) break;
            const auto size = static_cast<std::size_t>(got);
            if (size < kHead || std::memcmp(data, kMagic, 4u) != 0) continue;
            if (data[4] == kStatus) {
                // Answered only to a query at least as long as the answer, so
                // a forged sender gains nothing by asking.
                if (!options.status || size < std::max(kStatusQuery, kStatusAnswer)) continue;
                const std::size_t length = data[kHead + 4u];
                if (length > kLongestTag || size < kStatusQuery + length) continue;
                const Counts counts = options.status(
                    std::string_view(reinterpret_cast<const char *>(data + kStatusQuery), length));
                std::string answer = head(kStatus);
                put32(answer, get32(data + kHead));
                put32(answer, counts.version);
                put32(answer, counts.all);
                sendto(socket, answer.data(), static_cast<int>(answer.size()), 0,
                       reinterpret_cast<const sockaddr *>(&from), sizeof from);
                continue;
            }
            if (options.admit && !options.admit(from.sin_addr.s_addr)) continue;
            const std::uint32_t tag = tag_of(data);
            const auto pass = [&](const std::vector<End> &to) {
                for (const End &e : to)
                    if (!same_address(e.address, from))
                        sendto(socket, reinterpret_cast<const char *>(data), got, 0,
                               reinterpret_cast<const sockaddr *>(&e.address), sizeof e.address);
            };
            if (data[4] == kWatch) {
                // A watcher: kept apart from the two fighters, its WATCH
                // passed to both (the leading one streams from it).
                auto &list = watchers[tag];
                const auto me = std::find_if(list.begin(), list.end(),
                                             [&](const End &e) { return same_address(e.address, from); });
                if (me != list.end()) me->heard = Clock::now();
                else if (list.size() < kMostWatchers) list.push_back(End{from, Clock::now()});
                else continue;
                if (const auto found = ends.find(tag); found != ends.end()) pass(found->second);
                continue;
            }
            if (data[4] == kStream) {
                if (const auto found = watchers.find(tag); found != watchers.end()) pass(found->second);
                continue;
            }
            auto &list = ends[tag];
            End *const me = place(list, from);
            if (me == nullptr) continue;
            if (data[4] == kAsk) {
                // An introduction asked for: answered once both fighters have
                // asked, and never passed on.
                bool direct = false;
                Endpoint local;
                if (!read_ask(data, size, direct, local)) continue;
                me->asked = true;
                me->direct = direct;
                me->local = local;
                if (list.size() < 2u) continue;
                End &other = &list[0] == me ? list[1] : list[0];
                if (!other.asked) continue;
                Introduction intro;
                intro.direct = me->direct && other.direct &&
                               may_introduce(endpoint_of(me->address), endpoint_of(other.address));
                if (intro.direct) {
                    intro.seen = endpoint_of(other.address);
                    intro.local = other.local;
                }
                const std::string out = introduction(tag, intro);
                sendto(socket, out.data(), static_cast<int>(out.size()), 0, reinterpret_cast<const sockaddr *>(&from),
                       sizeof from);
                if (!me->told && !other.told)
                    std::cout << "[" << options.name << "] battle " << tag << ": "
                              << (intro.direct ? "the fighters may try a direct path"
                                               : "through the relay only (a fighter's choice, or this computer's)")
                              << std::endl;
                me->told = true;
                continue;
            }
            pass(list);
        }
    }
}

} // namespace

void start(Options options) {
    static std::once_flag once;
    std::call_once(once, [&] {
        // Its thread runs until the process ends, with the options it owns.
        std::thread([options = std::move(options)] { run(options); }).detach();
    });
}

std::string status_query(std::uint32_t nonce, std::string_view version) {
    if (version.size() > kLongestTag) version = version.substr(0, kLongestTag);
    std::string out = head(kStatus);
    put32(out, nonce);
    out.push_back(static_cast<char>(version.size()));
    out.append(version);
    if (out.size() < kStatusAnswer) out.append(kStatusAnswer - out.size(), '\0');
    return out;
}

bool read_status_answer(const std::uint8_t *data, std::size_t size, std::uint32_t nonce, Counts &counts) {
    if (size < kStatusAnswer || std::memcmp(data, kMagic, 4u) != 0 || data[4] != kStatus ||
        get32(data + kHead) != nonce)
        return false;
    counts.version = get32(data + kHead + 4u);
    counts.all = get32(data + kHead + 8u);
    return true;
}

std::string ask(std::uint32_t tag, bool direct, Endpoint local) {
    std::string out = head(kAsk, tag);
    out.push_back(direct ? '\1' : '\0');
    put_endpoint(out, local);
    return out;
}

bool read_ask(const std::uint8_t *data, std::size_t size, bool &direct, Endpoint &local) {
    if (size < kAskSize || std::memcmp(data, kMagic, 4u) != 0 || data[4] != kAsk) return false;
    direct = (data[kHead] & 1u) != 0u;
    local = get_endpoint(data + kHead + 1u);
    return true;
}

std::string introduction(std::uint32_t tag, const Introduction &intro) {
    std::string out = head(kIntro, tag);
    out.push_back(intro.direct ? '\1' : '\0');
    put_endpoint(out, intro.direct ? intro.seen : Endpoint{});
    put_endpoint(out, intro.direct ? intro.local : Endpoint{});
    return out;
}

bool read_introduction(const std::uint8_t *data, std::size_t size, Introduction &intro) {
    if (size < kIntroSize || std::memcmp(data, kMagic, 4u) != 0 || data[4] != kIntro) return false;
    intro.direct = (data[kHead] & 1u) != 0u;
    intro.seen = get_endpoint(data + kHead + 1u);
    intro.local = get_endpoint(data + kHead + 7u);
    if (intro.seen.address == 0u || intro.seen.port == 0u) intro.direct = false;
    return true;
}

bool may_introduce(Endpoint a, Endpoint b) {
    const auto loopback = [](Endpoint e) { return (ntohl(e.address) >> 24u) == 127u; };
    return loopback(a) == loopback(b);
}

} // namespace d012::net::relay
