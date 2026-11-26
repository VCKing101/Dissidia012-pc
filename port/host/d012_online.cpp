// Play online's server address and player count (d012_online.hpp).
//
// MIT licence; see port/README.md.

#if defined(_WIN32) && !defined(NOMINMAX)
#define NOMINMAX  // <windows.h>'s min and max macros break std::min under MSVC
#endif

#include "d012_online.hpp"

#include "d012_battle_relay.hpp"
#include "d012_net.hpp"
#include "d012_online_rules.hpp"
#include "d012_updates.hpp"

#include "adhoc/session.hpp"
#include "install/user_data.hpp"
#include "profile.hpp"
#include "settings/settings.hpp"

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <mutex>
#include <optional>
#include <random>
#include <thread>

namespace d012::online {
namespace {

using Clock = std::chrono::steady_clock;
using online_rules::address_in;

constexpr const char *kCacheName = "online-server.json";
// The count: asked this often while shown, kept this long, waited for this long.
constexpr auto kAskEvery = std::chrono::seconds(5);
constexpr auto kCountKept = std::chrono::seconds(20);
constexpr auto kAnswerWait = std::chrono::milliseconds(1500);
// Asked for within this long counts as shown.
constexpr auto kShown = std::chrono::seconds(3);

struct State {
    std::mutex mutex;
    std::string address;  // the server, empty while not known
    std::string note;     // why, while empty
    bool fetching{};
    int players = -1;
    Clock::time_point counted{};
    Clock::time_point wanted{};
    std::string asked;  // the server players() last asked about, for the count's thread
};

State &state() {
    static State *s = new State;  // its threads run until the process ends
    return *s;
}

const char *env(const char *name) {
    const char *text = std::getenv(name);
    return text != nullptr && *text != '\0' ? text : nullptr;
}

std::filesystem::path cache_path() {
    try {
        return portablekit::install::user_data_directory() / kCacheName;
    } catch (const std::exception &) {
        return {};
    }
}

std::string read_text(const std::filesystem::path &path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return {};
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

void set(const std::string &address, const std::string &note) {
    State &s = state();
    std::lock_guard lock(s.mutex);
    s.address = address;
    s.note = address.empty() ? note : std::string();
}

// Once a run: the kept answer at once, then the file.
void fetch() {
    const std::filesystem::path cache = cache_path();
    std::string kept_note;
    const std::string kept = cache.empty() ? std::string() : address_in(read_text(cache), kept_note);
    if (!kept.empty()) set(kept, {});

    const char *source = env("D012_ONLINE_URL");
    updates::Fetched answer;
    if (source != nullptr && std::strncmp(source, "https://", 8) != 0) {
        answer.body = read_text(source);
        answer.status = answer.body.empty() ? 0 : 200;
        if (answer.body.empty()) answer.problem = "D012_ONLINE_URL's file cannot be read";
    } else {
        answer = updates::get(source != nullptr ? source : kServerFile);
    }
    std::string note;
    std::string address;
    if (!answer.problem.empty()) note = "The server's address could not be fetched: " + answer.problem + ".";
    else if (answer.status != 200)
        note = "The server's address could not be fetched (GitHub answered " + std::to_string(answer.status) + ").";
    else address = address_in(answer.body, note);

    if (!address.empty() || note == online_rules::kNoServerYet) {
        // A good answer (an address, or none on purpose) replaces the kept one.
        if (!cache.empty()) {
            std::error_code error;
            std::filesystem::create_directories(cache.parent_path(), error);
            std::ofstream(cache, std::ios::binary) << answer.body;
        }
        set(address, note);
    } else if (kept.empty()) {
        set({}, note);
    }
    std::cout << "[online] server " << (address.empty() ? (kept.empty() ? "unknown" : kept + " (kept)") : address)
              << (note.empty() ? "" : ": " + note) << std::endl;
    State &s = state();
    std::lock_guard lock(s.mutex);
    s.fetching = false;
}

void start_fetch() {
    static std::once_flag once;
    std::call_once(once, [] {
        {
            State &s = state();
            std::lock_guard lock(s.mutex);
            s.fetching = true;
            s.note = "Finding the server…";
        }
        std::thread(fetch).detach();
    });
}

#if defined(_WIN32)
using Socket = SOCKET;
constexpr Socket kNoSocket = INVALID_SOCKET;
void close_socket(Socket s) { closesocket(s); }
bool sockets_ready() {
    static const bool started = [] {
        WSADATA wsa{};
        return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
    }();
    return started;
}
#else
using Socket = int;
constexpr Socket kNoSocket = -1;
void close_socket(Socket s) { close(s); }
bool sockets_ready() { return true; }
#endif

// One query to `server`'s battle port, the packet made by `make` from a
// number of its own: true once `read` took an answer to that number.
template <typename Make, typename Read>
bool query(const std::string &server, Make make, Read read) {
    std::string host = server;
    if (const std::size_t colon = host.rfind(':'); colon != std::string::npos && host.find(':') == colon)
        host.resize(colon);
    if (host.empty() || !sockets_ready()) return false;
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    addrinfo *found = nullptr;
    if (getaddrinfo(host.c_str(), nullptr, &hints, &found) != 0 || found == nullptr) return false;
    sockaddr_in to = *reinterpret_cast<const sockaddr_in *>(found->ai_addr);
    freeaddrinfo(found);
    to.sin_port = htons(net::battle::port());
    const Socket socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket == kNoSocket) return false;
    static std::mutex generator_mutex;
    static std::mt19937 generator{std::random_device{}()};
    std::uint32_t nonce;
    {
        std::lock_guard lock(generator_mutex);
        nonce = generator();
    }
    const std::string packet = make(nonce);
    bool answered = false;
    if (sendto(socket, packet.data(), static_cast<int>(packet.size()), 0, reinterpret_cast<const sockaddr *>(&to),
               sizeof to) == static_cast<int>(packet.size())) {
        const auto until = Clock::now() + kAnswerWait;
        while (!answered && Clock::now() < until) {
            fd_set set;
            FD_ZERO(&set);
            FD_SET(socket, &set);
            timeval wait{0, 100000};
            if (select(static_cast<int>(socket + 1), &set, nullptr, nullptr, &wait) <= 0) continue;
            std::uint8_t data[256];
            const int got = static_cast<int>(recv(socket, reinterpret_cast<char *>(data), sizeof data, 0));
            if (got > 0) answered = read(data, static_cast<std::size_t>(got), nonce);
        }
    }
    close_socket(socket);
    return answered;
}

// One STATUS query to `server`'s battle port: the count, or -1.
int ask(const std::string &server, const std::string &version) {
    int count = -1;
    query(
        server, [&](std::uint32_t nonce) { return net::relay::status_query(nonce, version); },
        [&](const std::uint8_t *data, std::size_t size, std::uint32_t nonce) {
            net::relay::Counts counts;
            if (!net::relay::read_status_answer(data, size, nonce, counts)) return false;
            count = static_cast<int>(counts.version);
            return true;
        });
    return count;
}

// Where everyone is, for the game's own screens: asked of the server the
// game goes online with while a screen asks for it.
constexpr auto kWhereEvery = std::chrono::seconds(3);
constexpr auto kWhereKept = std::chrono::seconds(15);

struct WhereState {
    std::mutex mutex;
    std::string server;  // the server last asked about, from the game's thread
    Clock::time_point wanted{};
    bool known{};
    net::relay::Where where;
    Clock::time_point answered{};
};

WhereState &where_state() {
    static WhereState *s = new WhereState;  // its thread runs until the process ends
    return *s;
}

void where_loop() {
    const std::string version = portablekit::adhoc_version_tag();
    for (;;) {
        WhereState &s = where_state();
        std::string server;
        {
            std::lock_guard lock(s.mutex);
            if (Clock::now() - s.wanted < kShown) server = s.server;
        }
        if (server.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            continue;
        }
        net::relay::Where where;
        const bool answered = query(
            server, [&](std::uint32_t nonce) { return net::relay::where_query(nonce, version); },
            [&](const std::uint8_t *data, std::size_t size, std::uint32_t nonce) {
                return net::relay::read_where_answer(data, size, nonce, where);
            });
        {
            std::lock_guard lock(s.mutex);
            if (answered && server == s.server) {
                s.known = true;
                s.where = where;
                s.answered = Clock::now();
            }
        }
        std::this_thread::sleep_for(kWhereEvery);
    }
}

void count_loop() {
    // The tag this copy logs in with, a mod's disc included: the count is of
    // the players it can meet.
    const std::string version = portablekit::adhoc_version_tag();
    for (;;) {
        State &s = state();
        bool shown;
        std::string server;
        {
            std::lock_guard lock(s.mutex);
            shown = Clock::now() - s.wanted < kShown;
            if (shown) server = s.asked;
        }
        if (!server.empty()) {
            const int count = ask(server, version);
            std::lock_guard lock(s.mutex);
            if (count >= 0) {
                s.players = count;
                s.counted = Clock::now();
            }
        }
        std::this_thread::sleep_for(shown ? kAskEvery : std::chrono::seconds(1));
    }
}

} // namespace

std::string public_server(std::string &note) {
    start_fetch();
    State &s = state();
    std::lock_guard lock(s.mutex);
    note = s.address.empty() ? s.note : std::string();
    return s.address;
}

int players() {
    static std::once_flag once;
    std::call_once(once, [] { std::thread(count_loop).detach(); });
    start_fetch();
    // The server Play online uses: the Online server setting, or the file's.
    // Read here, on the interface's thread, which is the one that changes it.
    const std::string setting = portablekit::settings::current().adhoc_online_server;
    State &s = state();
    std::lock_guard lock(s.mutex);
    const std::string server = setting.empty() ? s.address : setting;
    if (server != s.asked) {
        s.asked = server;
        s.players = -1;  // another server's count says nothing about this one
    }
    s.wanted = Clock::now();
    return s.players >= 0 && Clock::now() - s.counted < kCountKept ? s.players : -1;
}

bool where(const std::string &server, net::relay::Where &out) {
    static std::once_flag once;
    std::call_once(once, [] { std::thread(where_loop).detach(); });
    WhereState &s = where_state();
    std::lock_guard lock(s.mutex);
    if (server != s.server) {
        s.server = server;
        s.known = false;  // another server's answer says nothing about this one
    }
    s.wanted = Clock::now();
    if (!s.known || Clock::now() - s.answered >= kWhereKept) return false;
    out = s.where;
    return true;
}

} // namespace d012::online
