#pragma once

// The right-stick camera in battle: the right stick turns the battle camera
// and changes its distance as the D-pad does, in proportion to the push, and
// R3 recentres it. See docs/port/right-stick-camera.md. MIT licence; see
// port/README.md.

namespace portablekit {
class HleRegistrar;
}

namespace d012::camera {

// False with D012_NO_RIGHT_STICK_CAMERA=1: the game's own camera, untouched.
[[nodiscard]] bool enabled();
// The pad-read wrapper that keeps the right stick and gives R3 its meaning.
// The camera's two overrides go in from this file's static initialiser.
void install(portablekit::HleRegistrar &hle);

} // namespace d012::camera
