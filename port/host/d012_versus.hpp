#pragma once

// Local versus: player 2 on a second controller (docs/port/local-versus.md,
// section 1), reached from the Battle Mode row "Quick Battle (Player vs
// Player)" (d012_versus_menu.cpp). MIT licence; see port/README.md.

#include <cstdint>

namespace portablekit {
class HleRegistrar;
}

namespace d012::versus {

// D012_LOCAL_VERSUS: 0 leaves the game as it was (no row, no overrides);
// unset or anything else builds the mode in. 1 also makes every Quick Battle
// (1-on-1) a versus one, without the row (scripted tests).
[[nodiscard]] bool enabled();
[[nodiscard]] bool forced();
// A versus battle is being set up or played: chosen from the row (until the
// start menu comes back), or forced.
[[nodiscard]] bool active();
void set_chosen(bool chosen);
// The start menu is up: player 2's gamepad is looked for, so that the row
// knows whether there is one.
void note_start_menu();
[[nodiscard]] bool second_pad_connected();
// The notice "Connect a second controller", for a few seconds.
void ask_for_second_pad();

// Player 2's right stick as of the game's last pad read (centre 0x80), for
// the right-stick camera of the opponent's fighter (d012_camera.cpp). The
// camera calls note_camera_live() whenever player 2's camera takes input, so
// that R3 presses SELECT, the game's recentre, only then.
struct RightStick {
    std::uint8_t x = 0x80u;
    std::uint8_t y = 0x80u;
};
[[nodiscard]] RightStick player2_right_stick();
void note_camera_live();

// The pad-read wrapper: controller 2 into raw pad 1, and into the game's own
// pad while the opponent is picked. The opponent's AI-mode override goes in
// from this file's static initialiser.
void install(portablekit::HleRegistrar &hle);

} // namespace d012::versus
