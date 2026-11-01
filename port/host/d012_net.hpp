#pragma once

// Online play, delay-based lockstep (docs/port/online-play.md, section 12):
// two copies of the port swap each frame's input over UDP, and each frame
// runs only once both players' input for it is in, so both copies run the
// same frames with the same input. MIT licence; see port/README.md.

#include "d012_input.hpp"

#include <cstdint>
#include <string>

namespace d012::net {

// The environment variable `env` when set, else settings.ini's `key`, else
// empty: a copy started from a shortcut (or under Proton) has no environment
// of its own to give, so its online settings can live in settings.ini.
[[nodiscard]] std::string setting(const char *env, const char *key);

// What one copy adds to a frame: its player's pad, and the system dialogs'
// answers (taken from player 1's copy only; player 2's copy draws none).
struct Local {
    std::uint8_t dialog = 0;
    std::uint8_t save = 0;
    input::Pad pad;
};

// D012_NET turns it on:
// - D012_NET=host:PORT waits for the other copy on 127.0.0.1:PORT; this copy
//   is player 1, and its menus and dialogs are the game's;
// - D012_NET=join:ADDRESS:PORT joins one, as player 2.
// host:ADDRESS:PORT listens on another address (0.0.0.0 for a LAN, which
// Windows asks the player to allow). D012_NET_DELAY sets the frames of input
// delay (3 by default), D012_NET_WAIT the seconds to wait for the other copy
// (300). Player 1's copy picks the PSP clock's time and the save the title
// screen loads (D012_SAVEDATA_ANSWERS, "first" by default) and both copies take
// them, with the busy clock off, as section 10 needs.
// Returns whether the two copies are linked. Called once, at start-up, before
// d012::sync reads its settings.
bool start();

[[nodiscard]] bool active();
[[nodiscard]] bool player1();

// Read `read`: this copy's input goes out for read + delay, and the frame for
// `read` comes back once the other copy's input for it is in. If the other copy
// is lost, it goes on with player 2 idle.
input::Frame exchange(std::uint64_t read, const Local &mine);

// A hash of guest RAM the input record took at `read`, compared with the other
// copy's: a mismatch says the copies went apart, and when.
void check(std::uint64_t read, std::uint64_t hash);

// A Communication Mode battle's own link (d012_comm_battle.hpp): the two
// fighters' copies, each having reached the battle through the game's lobby,
// linked for that battle alone, its frames counted from the battle's start.
namespace battle {

// D012_NET_BATTLE=PORT or ADDRESS:PORT (or settings.ini's online.battle_link):
// where player 0's copy listens for player 1's (127.0.0.1 without an address;
// 0.0.0.0 listens on every network, which Windows asks the player to allow).
// Until the game's lobby hands the address over, both copies are given it.
// The input delay is player 0's D012_NET_DELAY or online.input_delay.
[[nodiscard]] bool configured();

// Links this copy, `player` 0 (which listens) or 1, for the battle `tag`
// (24 bits; both copies give the same). Waits up to D012_NET_WAIT seconds.
bool open(int player, std::uint32_t tag);
[[nodiscard]] bool open_now();

struct Pads {
    input::Pad player[2];
    bool lost = false;  // the other copy has not been heard from for 10 s
};
// As exchange() above, for the battle's frame `frame`.
Pads exchange(std::uint64_t frame, const input::Pad &mine);

// The game's own check value for `frame`, compared with the other copy's.
// False once the two have differed for some frame.
bool check(std::uint64_t frame, std::uint64_t hash);

void close();

} // namespace battle

} // namespace d012::net
