#pragma once

// Local versus: player 2 on a second controller (docs/port/local-versus.md,
// section 1). On only with D012_LOCAL_VERSUS=1 while the menu row is not
// built. MIT licence; see port/README.md.

#include <cstdint>

namespace portablekit {
class HleRegistrar;
}

namespace d012::versus {

// D012_LOCAL_VERSUS=1.
[[nodiscard]] bool on();

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
