#pragma once

// Installing a newer version from the menu, in a copy the Windows setup
// installed or in the player's Android app: downloads the release's program
// (the setup, the app), checks it against the release's signed list of files,
// and when the player says so installs it. See d012_update_install.cpp.
// MIT licence; see port/README.md.

#include "d012_update_rules.hpp"

#include <cstdint>
#include <string>

namespace d012::update_install {

enum class Step {
    Idle,
    Preparing,    // reading and checking the signed list
    Downloading,
    Checking,     // the download against the list
    Ready,        // the download is here and checked
    Installing,   // Android: its installer has it and asks the player
    Failed,
};

struct Status {
    Step step = Step::Idle;
    std::uint64_t done = 0;   // Downloading: bytes
    std::uint64_t total = 0;
    std::string version;      // what is being downloaded
    std::string problem;      // Failed: for the player; Ready: why it is not installed yet, or empty
};

// Whether this copy can install `release` itself: a copy the Windows setup
// installed, or the player's Android app. Any other copy, or a release that is
// not signed, is updated from the release's page.
[[nodiscard]] bool can_install(const update_rules::Release &release);

[[nodiscard]] Status status();

// Starts downloading on a thread of its own, unless that is under way.
void start(const update_rules::Release &release);
// Stops a download; what has arrived is kept for the next attempt.
void cancel();

// With the download Ready. Windows: starts the setup and closes the game.
// Android: hands the app to Android's installer, which asks the player and
// then stops the game to install it. False, with the status Failed, when that
// could not start.
bool install_now();

// At the program's start, before the game's code is loaded: true when the copy
// is new but its update never finished and the setup was started again, so
// this run should end (Windows). Also clears finished downloads, which is all
// it does on Android.
[[nodiscard]] bool finish_at_start();

} // namespace d012::update_install
