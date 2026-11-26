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
#include <climits>
#include <cstring>
#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <optional>
#include <sstream>
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

std::uint16_t get16(const std::uint8_t *data) {
    return static_cast<std::uint16_t>(data[0] | (data[1] << 8u));
}

std::uint64_t get64(const std::uint8_t *data) {
    return static_cast<std::uint64_t>(get32(data)) | (static_cast<std::uint64_t>(get32(data + 4u)) << 32u);
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

// --- The battles' log (Options::report_battles) -----------------------------
//
// Read from what the relay passes on, so it needs nothing of the copies. The
// battle link's INPUT (d012_net.cpp) after the head: u64 the other's frames
// held, u64 the first frame carried, u16 their count, ..., at +40 u32 the
// sender's clock in ms, at +44 u32 the other's clock from its latest INPUT
// plus how long the sender held it (~0 for none), at +48 s16 the sender's
// slack: how many ms before it needed them the other's inputs came (negative:
// it waited; INT16_MIN for none yet). BYE: a fighter left.
//
// A fighter's round trip to the relay comes from its echo: the relay noted
// when it passed on the other's INPUT stamped with that clock, so the echo
// coming back now, less the hold the echo adds, is relay -> fighter -> relay.
// The two fighters' round trips added are the battle's.
constexpr std::uint8_t kInput = 3u;
constexpr std::uint8_t kBye = 4u;
constexpr std::size_t kInputHead = kHead + 52u;
constexpr std::uint32_t kNoTime = ~0u;
constexpr std::int16_t kNoSlack = INT16_MIN;
constexpr auto kReportEvery = std::chrono::seconds(30);
constexpr auto kAfterBye = std::chrono::seconds(3);      // the other's BYE and last packets
constexpr auto kBattleQuiet = std::chrono::seconds(20);  // nothing at all: over (a direct battle ASKs every 5 s)
constexpr auto kRemembered = std::chrono::seconds(60);   // a reported battle's late packets start none
constexpr std::size_t kStamps = 256u;                    // over 4 s of a fighter's inputs
constexpr std::size_t kMostFighters = 4u;                // two, and a copy back on a new port

long long ms_of(Clock::duration d) { return std::chrono::duration_cast<std::chrono::milliseconds>(d).count(); }

std::string minutes(Clock::duration d) {
    const long long s = std::chrono::duration_cast<std::chrono::seconds>(d).count();
    std::ostringstream out;
    out << s / 60 << ':' << std::setw(2) << std::setfill('0') << s % 60;
    return out.str();
}

struct Tally {
    std::uint64_t inputs = 0;
    std::uint64_t frames_from = 0;  // the fighter's latest frame when it began
    bool frames_known = false;
    Clock::duration quiet{};        // the longest wait between two of its inputs
    std::uint64_t trips = 0;
    long long trip_total = 0;
    long long trip_least = LLONG_MAX;
    long long trip_most = 0;
    long long slack_total = 0;
    std::uint64_t slacks = 0;
    int slack_least = INT_MAX;
};

struct Fighter {
    sockaddr_in address{};
    std::string name;
    std::uint64_t inputs = 0;
    Clock::time_point first_input{};
    Clock::time_point last_input{};
    std::uint64_t latest = 0;  // its latest frame carried
    bool framed = false;
    Tally window;              // since the last report
    Tally all;                 // the whole battle
    std::deque<std::pair<std::uint32_t, Clock::time_point>> stamps;  // its clocks, when passed on
};

struct Battle {
    Clock::time_point began;
    Clock::time_point heard;
    Clock::time_point reported;
    std::optional<Clock::time_point> bye;
    bool relayed = false;  // the relay carried its input
    Clock::time_point first_relayed{};
    Clock::time_point last_relayed{};
    std::vector<Fighter> fighters;
};

Fighter *fighter_at(Battle &battle, const sockaddr_in &from, const Options &options) {
    for (Fighter &f : battle.fighters)
        if (same_address(f.address, from)) return &f;
    if (battle.fighters.size() >= kMostFighters) return nullptr;
    Fighter f;
    f.address = from;
    if (options.who) f.name = options.who(from.sin_addr.s_addr);
    battle.fighters.push_back(std::move(f));
    return &battle.fighters.back();
}

void note_input(Battle &battle, Fighter &me, const std::uint8_t *data, std::size_t size, Clock::time_point now) {
    if (size < kInputHead) return;
    const std::uint64_t first = get64(data + kHead + 8u);
    const std::uint32_t count = get16(data + kHead + 16u);
    const std::uint32_t stamp = get32(data + kHead + 40u);
    const std::uint32_t echo = get32(data + kHead + 44u);
    const auto slack = static_cast<std::int16_t>(get16(data + kHead + 48u));
    if (!battle.relayed) battle.first_relayed = now;
    battle.relayed = true;
    battle.last_relayed = now;
    if (me.inputs++ == 0u) me.first_input = now;
    else
        for (Tally *t : {&me.window, &me.all}) t->quiet = std::max(t->quiet, now - me.last_input);
    me.last_input = now;
    for (Tally *t : {&me.window, &me.all}) ++t->inputs;
    if (count != 0u) {
        me.latest = std::max(me.latest, first + count - 1u);
        for (Tally *t : {&me.window, &me.all}) {
            if (t->frames_known) continue;
            t->frames_known = true;
            t->frames_from = me.latest;
        }
        me.framed = true;
    }
    if (slack != kNoSlack)
        for (Tally *t : {&me.window, &me.all}) {
            t->slack_total += slack;
            ++t->slacks;
            t->slack_least = std::min(t->slack_least, static_cast<int>(slack));
        }
    if (echo != kNoTime) {
        for (const Fighter &other : battle.fighters) {
            if (&other == &me || other.stamps.empty()) continue;
            // The latest of the other's clocks at or below the echo: the one
            // it echoes, or one passed on just after it, which changes the
            // answer by only that packet's jitter.
            for (auto it = other.stamps.rbegin(); it != other.stamps.rend(); ++it) {
                const std::uint32_t held = echo - it->first;
                if (held > 5000u) continue;
                const long long trip = ms_of(now - it->second) - static_cast<long long>(held);
                if (trip >= 0 && trip < 10000)
                    for (Tally *t : {&me.window, &me.all}) {
                        ++t->trips;
                        t->trip_total += trip;
                        t->trip_least = std::min(t->trip_least, trip);
                        t->trip_most = std::max(t->trip_most, trip);
                    }
                break;
            }
            break;
        }
    }
    me.stamps.emplace_back(stamp, now);
    if (me.stamps.size() > kStamps) me.stamps.pop_front();
}

// "fighter 2" for a fighter with no name, "Player 2" for a name two share.
std::string name_of(const Battle &battle, std::size_t i) {
    const std::string &name = battle.fighters[i].name;
    const auto same = std::count_if(battle.fighters.begin(), battle.fighters.end(),
                                    [&](const Fighter &f) { return f.name == name; });
    if (!name.empty() && same == 1) return name;
    return (name.empty() ? std::string("fighter") : name) + " " + std::to_string(i + 1u);
}

// One fighter's part of a line: frames over `span`, round trip, silence, slack.
std::string tally_text(const Fighter &f, const Tally &t, Clock::duration span) {
    std::ostringstream out;
    if (t.inputs == 0u) return "nothing through the relay";
    const std::uint64_t frames = t.frames_known && f.latest >= t.frames_from ? f.latest - t.frames_from : 0u;
    const double seconds = std::chrono::duration<double>(span).count();
    out << frames << " frames";
    if (seconds >= 1.0) out << " (" << std::fixed << std::setprecision(1) << frames / seconds << "/s)";
    if (t.trips != 0u)
        out << ", round trip " << t.trip_total / static_cast<long long>(t.trips) << " ms (" << t.trip_least << "-"
            << t.trip_most << ")";
    out << ", quiet " << ms_of(t.quiet) << " ms at most";
    if (t.slacks != 0u)
        out << ", slack " << t.slack_total / static_cast<long long>(t.slacks) << " ms (least " << t.slack_least << ")";
    return out.str();
}

// Every 30 s while the relay carries the battle's input, up to `upto` (now, or
// the battle's end for its last part).
void report_window(const Options &options, std::uint32_t tag, Battle &battle, Clock::time_point upto) {
    const bool carried = std::any_of(battle.fighters.begin(), battle.fighters.end(),
                                     [](const Fighter &f) { return f.window.inputs != 0u; });
    if (carried && upto > battle.reported) {
        std::ostringstream out;
        out << "[" << options.name << "] battle " << tag << " at " << minutes(upto - battle.began) << ": ";
        for (std::size_t i = 0; i < battle.fighters.size(); ++i)
            out << (i == 0u ? "" : "; ") << name_of(battle, i) << " "
                << tally_text(battle.fighters[i], battle.fighters[i].window, upto - battle.reported);
        std::cout << out.str() << std::endl;
    }
    battle.reported = upto;
    for (Fighter &f : battle.fighters) {
        f.window = Tally{};
        if (!f.framed) continue;
        f.window.frames_known = true;
        f.window.frames_from = f.latest;
    }
}

// At its end: how long, which path, how it ended, and the whole battle's tally.
void report_end(const Options &options, std::uint32_t tag, Battle &battle, const char *how) {
    const Clock::time_point end = battle.bye ? *battle.bye : battle.heard;
    report_window(options, tag, battle, end);
    std::ostringstream out;
    out << "[" << options.name << "] battle " << tag << " ended after " << minutes(end - battle.began) << " (" << how
        << "): ";
    if (!battle.relayed) {
        out << "direct, the relay carried none of its input";
    } else {
        out << "through the relay";
        if (battle.first_relayed - battle.began > std::chrono::seconds(3))
            out << " from " << minutes(battle.first_relayed - battle.began);
        if (end - battle.last_relayed > std::chrono::seconds(3)) out << " until " << minutes(battle.last_relayed - battle.began);
        for (std::size_t i = 0; i < battle.fighters.size(); ++i) {
            const Fighter &f = battle.fighters[i];
            out << "; " << name_of(battle, i) << " " << tally_text(f, f.all, f.last_input - f.first_input);
        }
    }
    std::cout << out.str() << std::endl;
}

void run(const Options &options) {
    Socket socket = kNoSocket;
    auto checked = Clock::time_point{};
    std::map<std::uint32_t, std::vector<End>> ends;      // by tag, at most two
    std::map<std::uint32_t, std::vector<End>> watchers;  // by tag, the copies watching that battle
    std::map<std::uint32_t, Battle> battles;             // by tag, for the log
    std::map<std::uint32_t, Clock::time_point> finished; // by tag, battles already reported
    const auto report = [&](Clock::time_point now) {
        for (auto it = battles.begin(); it != battles.end();) {
            Battle &battle = it->second;
            const char *how = battle.bye && now - *battle.bye >= kAfterBye ? "a fighter left"
                              : now - battle.heard >= kBattleQuiet       ? "nothing came for 20 s"
                                                                         : nullptr;
            if (how == nullptr) {
                if (now - battle.reported >= kReportEvery) report_window(options, it->first, battle, now);
                ++it;
                continue;
            }
            report_end(options, it->first, battle, how);
            finished[it->first] = now;
            it = battles.erase(it);
        }
        for (auto it = finished.begin(); it != finished.end();)
            it = now - it->second > kRemembered ? finished.erase(it) : std::next(it);
    };
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
                for (auto &[tag, battle] : battles) report_end(options, tag, battle, "the relay closed");
                battles.clear();
                std::cout << "[" << options.name << "] closed" << std::endl;
            }
            forget(ends, now, std::chrono::seconds(60));
            forget(watchers, now, std::chrono::seconds(10));
            if (options.report_battles) report(now);
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
            if (data[4] == kWhere) {
                // As STATUS: only to a query as long as the answer.
                if (!options.where || size < std::max(kWhereQuery, kWhereAnswer)) continue;
                const std::size_t length = data[kHead + 4u];
                if (length > kLongestTag || size < kWhereQuery + length) continue;
                const Where where =
                    options.where(std::string_view(reinterpret_cast<const char *>(data + kWhereQuery), length));
                const std::string answer = where_answer(get32(data + kHead), where);
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
            if (options.report_battles && finished.count(tag) == 0u) {
                const auto now = Clock::now();
                const auto [found, added] = battles.try_emplace(tag);
                Battle &battle = found->second;
                if (added) battle.began = battle.reported = now;
                battle.heard = now;
                if (Fighter *const fighter = fighter_at(battle, from, options); fighter != nullptr) {
                    if (data[4] == kInput) note_input(battle, *fighter, data, size, now);
                    if (data[4] == kBye && !battle.bye) battle.bye = now;
                }
            }
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

namespace {

// Where's counts in the packet's order.
template <typename W, typename Each>
void each_count(W &where, Each each) {
    each(where.online);
    each(where.menus);
    for (auto &lobby : where.lobbies) each(lobby);
    each(where.group_battle);
    each(where.duels);
    each(where.party_battles);
    each(where.communications);
}

} // namespace

std::string where_query(std::uint32_t nonce, std::string_view version) {
    if (version.size() > kLongestTag) version = version.substr(0, kLongestTag);
    std::string out = head(kWhere);
    put32(out, nonce);
    out.push_back(static_cast<char>(version.size()));
    out.append(version);
    if (out.size() < kWhereAnswer) out.append(kWhereAnswer - out.size(), '\0');
    return out;
}

std::string where_answer(std::uint32_t nonce, const Where &where) {
    std::string out = head(kWhere);
    put32(out, nonce);
    each_count(where, [&](std::uint16_t value) {
        out.push_back(static_cast<char>(value & 0xFFu));
        out.push_back(static_cast<char>(value >> 8u));
    });
    return out;
}

bool read_where_answer(const std::uint8_t *data, std::size_t size, std::uint32_t nonce, Where &where) {
    if (size < kWhereAnswer || std::memcmp(data, kMagic, 4u) != 0 || data[4] != kWhere ||
        get32(data + kHead) != nonce)
        return false;
    const std::uint8_t *at = data + kHead + 4u;
    each_count(where, [&](std::uint16_t &value) {
        value = get16(at);
        at += 2u;
    });
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
