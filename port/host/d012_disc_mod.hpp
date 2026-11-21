#pragma once

// A disc mod: a patched image of the game's disc (Emendatus, for one) played
// on this port's own compiled code, switched on and off on the menu's System
// page and taking effect when the game restarts. The patched edition itself is
// PortableKit's (host/patched_edition.hpp): the executable and overlays are
// compared with the game's own, and only the code the mod changed runs
// interpreted. See d012_disc_mod.cpp. MIT licence; see port/README.md.

#include <filesystem>

namespace portablekit {
class HleRegistrar;
}

namespace d012::disc_mod {

// The patched disc this start plays (the profile's patched_disc): the image
// chosen on the System page while its switch is on; empty for the game's own.
[[nodiscard]] std::filesystem::path disc_for_this_start();

// Adds the System page's rows. Once.
void install(portablekit::HleRegistrar &hle);

} // namespace d012::disc_mod
