#pragma once

// Installing a newer version from the menu, in a copy the Windows setup
// installed: downloads the release's setup, checks it against the release's
// signed list of files, and when the player says so closes the game and lets
// the setup update the copy and start it again. See d012_update_install.cpp.
// MIT licence; see port/README.md.

#include "d012_update_rules.hpp"

#include <cstdint>
#include <string>

namespace d012::update_install {

enum class Step {
    Idle,
    Preparing,    // reading and checking the signed list
    Downloading,
    Checking,     // the downloaded setup against the list
    Ready,        // the setup is here and checked
    Failed,
};

struct Status {
    Step step = Step::Idle;
    std::uint64_t done = 0;   // Downloading: bytes
    std::uint64_t total = 0;
    std::string version;      // what is being downloaded
    std::string problem;      // Failed: for the player
};

// Whether this copy can install `release` itself. A copy that came from
// anywhere but the Windows setup, or a release that is not signed, is updated
// from the release's page.
[[nodiscard]] bool can_install(const update_rules::Release &release);

[[nodiscard]] Status status();

// Starts downloading on a thread of its own, unless that is under way.
void start(const update_rules::Release &release);
// Stops a download; what has arrived is kept for the next attempt.
void cancel();

// With the setup Ready: starts it and closes the game. False, with the status
// Failed, when it could not be started.
bool install_now();

// At the program's start, before the game's code is loaded: true when the copy
// is new but its update never finished and the setup was started again, so
// this run should end. Also clears finished downloads.
[[nodiscard]] bool finish_at_start();

} // namespace d012::update_install
