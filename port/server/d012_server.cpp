// dissidia012-server: the public online server (docs/port/online-matchmaking.md,
// sections 8 and 9). It runs what a copy hosting a session runs, for everyone
// and all the time, with nothing of the game:
// - PortableKit's ad hoc server as a public one (TCP 27312, its relay 27313):
//   copies on different versions kept apart, nobody shown another player's
//   address, a player's MAC and relay sockets kept theirs, per-address limits;
// - the battle relay (UDP 27420, port/host/d012_battle_relay.hpp), for copies
//   whose players are logged in only, and the STATUS answer: the players
//   online with a copy's version;
// - a ban list, a file of addresses and copies' MACs, read again when it
//   changes.
//
//   dissidia012-server [--port N] [--battle-port N] [--ban-list FILE]
//                      [--max-players N] [--per-address N] [--rate KB]
//                      [--loopback]
//
// MIT licence; see port/README.md.

#include "d012_battle_relay.hpp"

#include "adhoc/protocol.hpp"
#include "adhoc/server.hpp"
#include "profile.hpp"

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

#include <algorithm>
#include <atomic>
#include <cctype>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <thread>

namespace portablekit {

// PortableKit's ad hoc code asks the game's profile for its names; the server
// has no game, so it gives only those (the rest left empty on purpose).
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
const GameProfile &game() {
    static const GameProfile profile{
        .app_name = "dissidia012-server",
        .project_name = "Dissidia 012 server",
        .env_prefix = "D012",
        .adhoc_product_code = "ULUS10566",
    };
    return profile;
}

} // namespace portablekit

namespace {

using portablekit::adhoc::Mac;
using Clock = std::chrono::steady_clock;

std::atomic<bool> g_quit{false};

void on_signal(int) { g_quit = true; }

std::string trim(std::string text) {
    const auto space = [](unsigned char c) { return std::isspace(c) != 0; };
    text.erase(text.begin(), std::find_if_not(text.begin(), text.end(), space));
    text.erase(std::find_if_not(text.rbegin(), text.rend(), space).base(), text.end());
    return text;
}

// "02:11:22:33:44:0a" or with dashes.
std::optional<Mac> parse_mac(const std::string &text) {
    if (text.size() != 17u) return std::nullopt;
    Mac mac{};
    for (std::size_t i = 0; i < 6u; ++i) {
        const std::size_t at = i * 3u;
        if (i > 0u && text[at - 1u] != ':' && text[at - 1u] != '-') return std::nullopt;
        if (!std::isxdigit(static_cast<unsigned char>(text[at])) ||
            !std::isxdigit(static_cast<unsigned char>(text[at + 1u])))
            return std::nullopt;
        mac[i] = static_cast<std::uint8_t>(std::stoul(text.substr(at, 2u), nullptr, 16));
    }
    return mac;
}

// The owner's list of who is turned away: one address (as the log shows it) or
// one MAC a line; # starts a comment. Asked from the servers' threads.
class BanList {
public:
    explicit BanList(std::filesystem::path path) : path_(std::move(path)) {}

    // Reads the file again if it changed; on the main thread.
    void reload_if_changed() {
        if (path_.empty()) return;
        std::error_code error;
        const bool exists = std::filesystem::exists(path_, error);
        const auto stamp = exists ? std::filesystem::last_write_time(path_, error) : std::filesystem::file_time_type{};
        if (loaded_ && exists == existed_ && stamp == stamp_) return;
        loaded_ = true;
        existed_ = exists;
        stamp_ = stamp;
        std::set<std::string> addresses;
        std::set<Mac> macs;
        std::ifstream in(path_);
        std::string line;
        std::size_t number = 0;
        while (std::getline(in, line)) {
            ++number;
            if (const std::size_t hash = line.find('#'); hash != std::string::npos) line.resize(hash);
            line = trim(line);
            if (line.empty()) continue;
            if (const auto mac = parse_mac(line)) macs.insert(*mac);
            else if (line.find_first_of(" \t") == std::string::npos) addresses.insert(line);
            else std::cout << "[ban-list] line " << number << " is neither an address nor a MAC; left out" << std::endl;
        }
        {
            std::lock_guard lock(mutex_);
            addresses_ = std::move(addresses);
            macs_ = std::move(macs);
        }
        std::cout << "[ban-list] " << (exists ? "read " : "no file at ") << path_.string() << ": "
                  << addresses_count() << " addresses and " << macs_count() << " copies turned away" << std::endl;
    }
    bool address(const std::string &text) const {
        std::lock_guard lock(mutex_);
        return addresses_.count(text) != 0u;
    }
    bool mac(const Mac &mac) const {
        std::lock_guard lock(mutex_);
        return macs_.count(mac) != 0u;
    }

private:
    std::size_t addresses_count() const {
        std::lock_guard lock(mutex_);
        return addresses_.size();
    }
    std::size_t macs_count() const {
        std::lock_guard lock(mutex_);
        return macs_.size();
    }
    std::filesystem::path path_;
    bool loaded_{};
    bool existed_{};
    std::filesystem::file_time_type stamp_{};
    mutable std::mutex mutex_;
    std::set<std::string> addresses_;
    std::set<Mac> macs_;
};

// Who is online, for the battle relay's thread: the addresses players log in
// from (only they may use the relay) and the count on each version.
class Online {
public:
    void update(const portablekit::adhoc::ServerStatus &status) {
        std::set<std::uint32_t> addresses;
        std::map<std::string, std::uint32_t> versions;
        for (const auto &player : status.players) {
            in_addr v4{};
            if (inet_pton(AF_INET, player.address.c_str(), &v4) == 1) addresses.insert(v4.s_addr);
            ++versions[player.version];
        }
        std::lock_guard lock(mutex_);
        addresses_ = std::move(addresses);
        versions_ = std::move(versions);
        all_ = static_cast<std::uint32_t>(status.players.size());
    }
    bool admits(std::uint32_t address) const {
        std::lock_guard lock(mutex_);
        return addresses_.count(address) != 0u;
    }
    d012::net::relay::Counts counts(std::string_view version) const {
        std::lock_guard lock(mutex_);
        d012::net::relay::Counts counts;
        if (const auto found = versions_.find(std::string(version)); found != versions_.end())
            counts.version = found->second;
        counts.all = all_;
        return counts;
    }
    std::string summary() const {
        std::lock_guard lock(mutex_);
        std::ostringstream out;
        out << all_ << " players online";
        for (const auto &[version, count] : versions_)
            out << (version == versions_.begin()->first ? ": " : ", ") << count << " on "
                << (version.empty() ? std::string("no version") : version);
        return out.str();
    }

private:
    mutable std::mutex mutex_;
    std::set<std::uint32_t> addresses_;
    std::map<std::string, std::uint32_t> versions_;
    std::uint32_t all_{};
};

void usage() {
    std::cout << "dissidia012-server: the public online server for Dissidia 012's PC port.\n\n"
                 "  --port N         the lobby's TCP port (default 27312); its relay is the next one up\n"
                 "  --battle-port N  the battle relay's UDP port (default 27420)\n"
                 "  --ban-list FILE  addresses and copies' MACs to turn away, one a line (# for comments),\n"
                 "                   read again whenever it changes\n"
                 "  --max-players N  players online at most (default 512)\n"
                 "  --per-address N  players from one address at most (default 8)\n"
                 "  --rate KB        what one address may send, in KB a second (default 512)\n"
                 "  --loopback       listen on 127.0.0.1 alone, for copies on this computer\n";
}

bool number(const char *text, long low, long high, long &out) {
    char *end = nullptr;
    const long value = std::strtol(text, &end, 10);
    if (end == text || *end != '\0' || value < low || value > high) return false;
    out = value;
    return true;
}

} // namespace

int main(int argc, char **argv) {
    long port = portablekit::adhoc::kAdhocctlPort;
    long battle_port = 27420;
    long max_players = 512;
    long per_address = 8;
    long rate_kb = 512;
    bool loopback = false;
    std::filesystem::path ban_path;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        const bool has_value = i + 1 < argc;
        if (arg == "--help" || arg == "-h") {
            usage();
            return 0;
        } else if (arg == "--loopback") {
            loopback = true;
        } else if (arg == "--port" && has_value && number(argv[++i], 1024, 65534, port)) {
        } else if (arg == "--battle-port" && has_value && number(argv[++i], 1024, 65535, battle_port)) {
        } else if (arg == "--max-players" && has_value && number(argv[++i], 1, 100000, max_players)) {
        } else if (arg == "--per-address" && has_value && number(argv[++i], 1, 1000, per_address)) {
        } else if (arg == "--rate" && has_value && number(argv[++i], 1, 1000000, rate_kb)) {
        } else if (arg == "--ban-list" && has_value) {
            ban_path = argv[++i];
        } else {
            std::cerr << "dissidia012-server: cannot use " << arg << "\n\n";
            usage();
            return 2;
        }
    }

    BanList bans(ban_path);
    bans.reload_if_changed();
    Online online;

    portablekit::adhoc::ServerConfig config;
    config.adhocctl_port = static_cast<std::uint16_t>(port);
    config.loopback_only = loopback;
    config.print_events = true;
    config.public_server = true;
    config.max_logins = static_cast<std::size_t>(max_players);
    // Each copy opens a relay connection per ad hoc socket of the game's: a
    // few, more during a Group Battle.
    config.max_relays = static_cast<std::size_t>(max_players) * 16u;
    config.max_logins_per_address = static_cast<std::size_t>(per_address);
    config.max_relays_per_address = static_cast<std::size_t>(per_address) * 32u;
    config.bytes_per_second_per_address = static_cast<std::size_t>(rate_kb) * 1024u;
    config.refuse_address = [&bans](const std::string &address) { return bans.address(address); };
    config.refuse_mac = [&bans](const Mac &mac) { return bans.mac(mac); };
    portablekit::adhoc::Server server;
    if (!server.start(config)) {
        std::cerr << "dissidia012-server: " << server.status().error << std::endl;
        return 1;
    }

    d012::net::relay::Options relay;
    relay.port = static_cast<std::uint16_t>(battle_port);
    relay.loopback_only = loopback;
    relay.admit = [&online](std::uint32_t address) { return online.admits(address); };
    relay.status = [&online](std::string_view version) { return online.counts(version); };
    relay.name = "battle-relay";
    d012::net::relay::start(std::move(relay));

    std::signal(SIGINT, on_signal);
    std::signal(SIGTERM, on_signal);
    std::cout << "dissidia012-server: lobby on TCP " << port << " and " << portablekit::adhoc::relay_port_for(
                     static_cast<std::uint16_t>(port))
              << ", battles on UDP " << battle_port << (loopback ? ", on 127.0.0.1 alone" : "")
              << ". Ctrl+C stops it." << std::endl;

    auto banned_at = Clock::now();
    auto summary_at = Clock::now();
    std::string last_summary;
    while (!g_quit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        const auto now = Clock::now();
        online.update(server.status());
        if (now - banned_at >= std::chrono::seconds(5)) {
            banned_at = now;
            bans.reload_if_changed();
        }
        // Who is online, when it changed, at most once a minute.
        if (now - summary_at >= std::chrono::minutes(1)) {
            summary_at = now;
            std::string summary = online.summary();
            if (summary != last_summary) std::cout << "[server] " << summary << std::endl;
            last_summary = std::move(summary);
        }
    }
    std::cout << "dissidia012-server: stopping" << std::endl;
    server.stop();
    return 0;
}
