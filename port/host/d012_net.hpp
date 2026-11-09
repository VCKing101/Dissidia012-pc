#pragma once

// Online play, delay-based lockstep (docs/port/online-play.md, section 12):
// two copies of the port swap each frame's input over UDP, and each frame
// runs only once both players' input for it is in, so both copies run the
// same frames with the same input. MIT licence; see port/README.md.

#include "d012_input.hpp"

#include <optional>
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
// Without it, both fighters' copies go through the relay of the copy that
// hosts the game's session (serve()), at port() of the session's address, and
// player 0's copy leads the link. The leading copy's D012_NET_DELAY or
// online.input_delay sets the input delay: a number of frames, or by default
// "auto", from the round trip timed as the two link.
[[nodiscard]] bool configured();

// The battle link's UDP port: D012_NET_BATTLE_PORT or online.battle_port,
// 27420 by default. A hosting copy asks the router to forward it.
[[nodiscard]] std::uint16_t port();
// While this copy hosts the session, relay its battles on port() (none with
// online.battle_link set): the two fighters need not be this copy.
void serve();

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
// The game's player 0 or 1's, while a battle is linked or watched.
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
    // A watcher's frame to run without showing or playing it, as fast as it
    // can (portablekit set_simulate_only): it skips ahead to the live game.
    bool unseen = false;
};
// As exchange() above, for the battle's frame `frame`.
Pads exchange(std::uint64_t frame, const input::Pad &mine);

// Rollback (D012_NET_ROLLBACK or online.rollback): how many frames this copy
// may run past the other's latest input, guessing it; 0, the default, keeps
// the lockstep. The guessed frames come off the automatic input delay.
[[nodiscard]] std::uint32_t rollback_frames();
// Rollback's exchange: both pads for `frame` without waiting for the other's,
// guessed (`guessed`) when it has not come. A frame run again (`rerun`) keeps
// the input this copy sent for it, sends nothing and never waits.
Pads exchange_ahead(std::uint64_t frame, const input::Pad &mine, bool rerun, bool &guessed);
// The earliest frame whose guess the other's real input has shown wrong since
// the last call, if any: the game goes back to it and runs on from there.
[[nodiscard]] std::optional<std::uint64_t> take_wrong_guess();
// The other copy's frames held: every frame below this one is certain.
[[nodiscard]] std::uint64_t confirmed();

// The game's own check value for `frame`, compared with the other copy's.
// False once the two have differed for some frame.
bool check(std::uint64_t frame, std::uint64_t hash);

// Ends the battle's link, `reached` the frame this copy's battle had reached.
// The leading copy first gives its watchers the rest of the battle.
void close(std::uint64_t reached);

// Watching a battle (Group Battle's members who are not fighting,
// docs/port/communication-mode.md section 19). The leading fighter's copy
// streams both players' certain pads, both camera settings and its check
// values through the session host's relay to every copy that watches the
// battle's tag, from the first frame each watcher lacks. Only through a relay:
// with online.battle_link set there is none, and watched battles stay on the
// game's own exchange.
[[nodiscard]] bool can_stream();
// Starts watching the battle `tag`; false when no socket or relay address.
bool watch(std::uint32_t tag);
[[nodiscard]] bool watching();
// Both players' pads for `frame`, once the stream has brought them (waiting
// with a screen after half a second, as a fighter does). The watcher keeps a
// buffer of frames in hand sized to its path; a frame that has not come
// waits until the buffer is full again, and a watcher well behind the buffer
// runs the extra frames `unseen`. `lost` when the stream ended before `frame`
// (the battle was quit there), stopped coming, or the player chose to leave
// (`left`).
Pads watch_frame(std::uint64_t frame);
// This copy's check value for `frame`, compared with the leading fighter's
// whenever both are in. False once they have differed.
bool watch_check(std::uint64_t frame, std::uint32_t value);
void unwatch();

} // namespace battle

} // namespace d012::net
