#pragma once

// The right-stick camera in battle: the right stick turns the battle camera
// and changes its distance as the D-pad does, in proportion to the push, and
// R3 recentres it. See docs/port/right-stick-camera.md. MIT licence; see
// port/README.md.

#include <cstdint>

namespace portablekit {
class HleRegistrar;
}

namespace d012::camera {

// False with D012_NO_RIGHT_STICK_CAMERA=1: the game's own camera, untouched.
[[nodiscard]] bool enabled();
// Adds the mouse's motion since the last call to player 1's right stick,
// given as a pad sample's bytes 10 and 11: the push a camera update gets
// from it, sized by the local camera (d012_camera.cpp, "The mouse"). Once
// per camera update: the local camera calls it itself, and in a linked
// battle the link does before sending the pad. Nothing with
// D012_NO_MOUSE_CAMERA=1 (or without the right-stick camera).
void add_mouse(std::uint8_t &x, std::uint8_t &y);
// The pad-read wrapper that keeps the right stick and gives R3 its meaning.
// The camera's two overrides go in from this file's static initialiser.
void install(portablekit::HleRegistrar &hle);

} // namespace d012::camera
