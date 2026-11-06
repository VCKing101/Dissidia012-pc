#pragma once

// Checking for a newer version: GitHub's latest release against this
// program's version (d012_version.hpp), on the menu's System page and, unless
// the player turns it off, once when the game starts, with a notice over the
// title screen. It only says so and opens the release's page; the player
// downloads and runs the setup as for any version. See d012_updates.cpp.
// MIT licence; see port/README.md.

namespace portablekit {
class HleRegistrar;
}

namespace d012::updates {

// Adds the System page's rows and the notice. Once.
void install(portablekit::HleRegistrar &hle);

} // namespace d012::updates
