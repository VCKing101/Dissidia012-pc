#pragma once

// Online play, delay-based lockstep (docs/port/online-play.md, section 12):
// two copies of the port swap each frame's input over UDP, and each frame
// runs only once both players' input for it is in, so both copies run the
// same frames with the same input. MIT licence; see port/README.md.

#include "d012_input.hpp"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

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

// On with ad hoc play (network.adhoc), or with D012_NET_BATTLE=PORT or
// ADDRESS:PORT (settings.ini's online.battle_link), a test switch: where
// player 0's copy listens for player 1's (127.0.0.1 without an address).
// Without it, the copy that hosts the game's session listens on every network
// at port() (which Windows asks the player to allow), and the other joins the
// session's address there. The listening copy's D012_NET_DELAY or
// online.input_delay sets the input delay: a number of frames, or by default
// "auto", from the round trip timed as the two link.
[[nodiscard]] bool configured();

// The battle link's UDP port: D012_NET_BATTLE_PORT or online.battle_port,
// 27420 by default. A hosting copy asks the router to forward it.
[[nodiscard]] std::uint16_t port();

// What must be the same on both copies for their battles to stay in step: the
// program, mods that change the game, settings that change the fight. `what`
// is how the player reads it ("the port's version"). The listening copy
// compares both lists as the battle links and refuses a copy that differs.
// Registered at start-up.
void must_match(const std::string &what, const std::string &value);
// Every item registered, in order: for a problem report.
[[nodiscard]] std::vector<std::pair<std::string, std::string>> must_match_items();

// A player's right-stick camera settings. Each copy sends its own as the
// battle links, and both copies turn each fighter's camera with that player's.
struct Camera {
    float dead_zone = 0.18f;
    float sensitivity = 1.0f;
    bool invert_x = false;
    bool invert_y = false;
};
void set_camera(const Camera &mine);
// The game's player 0 or 1's, while a battle is linked.
[[nodiscard]] Camera camera(int player);

// Links this copy, the game's `player` 0 or 1, for the battle `tag` (24 bits;
// both copies give the same). Waits up to D012_NET_WAIT seconds (30 by
// default); after half a second a screen says so and lets the player leave.
bool open(int player, std::uint32_t tag);
[[nodiscard]] bool open_now();

// Why the last open() failed, for the player: the other copy differs, or never
// answered. Empty when it did not fail, or the player chose to leave.
[[nodiscard]] std::string why_not();

// Shows `text` over the game with an OK, and waits for it. Nothing without an
// interface.
void notice(const std::string &text);

struct Pads {
    input::Pad player[2];
    bool lost = false;  // the other copy is gone, or no frame of it came for 30 s
    bool left = false;  // this copy's player chose to leave while it waited
};
// As exchange() above, for the battle's frame `frame`.
Pads exchange(std::uint64_t frame, const input::Pad &mine);

// The game's own check value for `frame`, compared with the other copy's.
// False once the two have differed for some frame.
bool check(std::uint64_t frame, std::uint64_t hash);

void close();

} // namespace battle

} // namespace d012::net
