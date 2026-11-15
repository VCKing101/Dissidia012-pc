#pragma once

// The battle relay (docs/port/online-next.md, section 2): a UDP socket that
// passes each battle link's datagrams between its two copies, told apart by
// the link's tag, so neither fighter needs a reachable address. Copies
// watching a battle (WATCH) are kept apart from its two fighters: their WATCH
// goes to both fighters, and the leading fighter's STREAM to them.
//
// The game runs one while it hosts a session (d012_net.cpp); the public server
// (port/server, docs/port/online-matchmaking.md, section 8) runs one all the
// time, and answers STATUS there:
// - STATUS (11), a copy to the relay: "D0NL", 11, tag 0, u32 a number of the
//   copy's choosing, u8 length and a version tag (GameProfile::adhoc_version).
// - The answer: "D0NL", 11, tag 0, the same u32, u32 the players online with
//   that tag, u32 the players online in all.
//
// It needs nothing of PortableKit's, so the server program builds it alone.
// MIT licence; see port/README.md.

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

namespace d012::net::relay {

// The battle link's framing (d012_net.cpp): "D0NL", u8 kind, a 24-bit tag.
inline constexpr char kMagic[4] = {'D', '0', 'N', 'L'};
inline constexpr std::size_t kHead = 8u;
inline constexpr std::uint8_t kWatch = 9u;
inline constexpr std::uint8_t kStream = 10u;
inline constexpr std::uint8_t kStatus = 11u;
inline constexpr std::size_t kStatusQuery = kHead + 5u;    // and the tag's bytes
inline constexpr std::size_t kStatusAnswer = kHead + 12u;

struct Counts {
    std::uint32_t version{};  // players online with the asked tag
    std::uint32_t all{};      // players online in all
};

struct Options {
    std::uint16_t port{};
    // Listens on 127.0.0.1 alone (copies on one computer; no firewall asks).
    bool loopback_only{};
    // Whether the socket should be open now, asked twice a second; closing it
    // forgets every battle. Null: always open.
    std::function<bool()> open;
    // Whether datagrams from this IPv4 address (network byte order) may use
    // the relay. STATUS is answered whatever it says. Null: any address.
    std::function<bool(std::uint32_t address)> admit;
    // The answer to STATUS for a version tag. Null: STATUS goes unanswered.
    std::function<Counts(std::string_view version)> status;
    // How the log names it.
    std::string name = "battle-relay";
};

// Starts the relay on a thread of its own that runs until the process ends.
// Only the first call in a process starts one.
void start(Options options);

// Packets for STATUS: a query, and an answer read back (false when `data` is
// not one, or answers another query).
[[nodiscard]] std::string status_query(std::uint32_t nonce, std::string_view version);
[[nodiscard]] bool read_status_answer(const std::uint8_t *data, std::size_t size, std::uint32_t nonce, Counts &counts);

} // namespace d012::net::relay
