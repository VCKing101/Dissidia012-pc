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
// Direct battles (docs/port/online-matchmaking.md, section 13): each fighter
// asks the relay for an introduction, and the relay, once both fighters of a
// tag have asked, answers each with the other's address, or with none when
// either wants every battle through the relay. Neither packet is passed on.
// - ASK (12), a fighter to the relay: "D0NL", 12, the battle's tag, u8 1 when
//   it may go direct (0: Always through the server), u32 its address on its own
//   network and u16 its port (network byte order).
// - INTRO (13), the relay's answer: "D0NL", 13, the tag, u8 1 when the two may
//   try a direct path (0: through the relay only, and no address follows but
//   zeros), then the other fighter's address and port as the relay saw them,
//   and its address and port on its own network (u32, u16 each, network order).
// A fighter asks again every few seconds while its battle goes direct: it keeps
// its place at the relay, and its router's path to it, for a move back.
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
inline constexpr std::uint8_t kAsk = 12u;
inline constexpr std::uint8_t kIntro = 13u;
inline constexpr std::size_t kStatusQuery = kHead + 5u;    // and the tag's bytes
inline constexpr std::size_t kStatusAnswer = kHead + 12u;
inline constexpr std::size_t kAskSize = kHead + 7u;
inline constexpr std::size_t kIntroSize = kHead + 13u;

struct Counts {
    std::uint32_t version{};  // players online with the asked tag
    std::uint32_t all{};      // players online in all
};

// An IPv4 address and a port, both in network byte order (as in sockaddr_in).
struct Endpoint {
    std::uint32_t address{};
    std::uint16_t port{};
    friend bool operator==(const Endpoint &, const Endpoint &) = default;
};

struct Introduction {
    bool direct{};  // the two may try a direct path
    Endpoint seen;  // the other fighter as the relay saw it
    Endpoint local; // and on its own network, as it said
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

// Packets for a direct battle: ASK and INTRO written, and read back (false
// when `data` is not one).
[[nodiscard]] std::string ask(std::uint32_t tag, bool direct, Endpoint local);
[[nodiscard]] bool read_ask(const std::uint8_t *data, std::size_t size, bool &direct, Endpoint &local);
[[nodiscard]] std::string introduction(std::uint32_t tag, const Introduction &intro);
[[nodiscard]] bool read_introduction(const std::uint8_t *data, std::size_t size, Introduction &intro);
// Whether the relay may give each of two fighters, seen at `a` and `b`, the
// other's address: both on this computer (127.x) or neither. A fighter on the
// relay's own computer reached it by its loopback address, which means
// nothing to the other; theirs is a battle the relay carries anyway.
[[nodiscard]] bool may_introduce(Endpoint a, Endpoint b);

} // namespace d012::net::relay
