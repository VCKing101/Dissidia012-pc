#pragma once

// Checking for a newer version: GitHub's latest release against this
// program's version (d012_version.hpp), on the menu's System page and, unless
// the player turns it off, once when the game starts, with a notice over the
// title screen. It says so and opens the release's page; a copy the Windows
// setup installed can also install the release from the System page
// (d012_update_install.hpp). See d012_updates.cpp.
// MIT licence; see port/README.md.

#include <string>

namespace portablekit {
class HleRegistrar;
}

namespace d012::updates {

// Adds the System page's rows and the notice. Once.
void install(portablekit::HleRegistrar &hle);

// One HTTPS GET as the check makes it (WinHTTP, Java or the system's libcurl),
// on the caller's thread: it can take 20 s. For the online server's address
// too (d012_online.cpp).
struct Fetched {
    long status = 0;      // HTTP status, 0 when none came
    std::string body;     // at most 1 MiB
    std::string problem;  // for the player, when it could not be fetched
};
[[nodiscard]] Fetched get(const std::string &url);

} // namespace d012::updates
