#pragma once

// Installing a newer version from the menu, in a copy the Windows or Linux
// setup installed or in the player's Android app: downloads the release's
// program (the setup, the app), checks it against the release's signed list of
// files, and when the player says so installs it. See d012_update_install.cpp.
// MIT licence; see port/README.md.

#include "d012_update_rules.hpp"

#include <cstdint>
#include <filesystem>
#include <string>

namespace portablekit::install {
class InstallerUi;
}

namespace d012::update_install {

enum class Step {
    Idle,
    Preparing,    // reading and checking the signed list
    Downloading,
    Checking,     // the download against the list
    Unpacking,    // Linux: the program's files out of the AppImage
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

// Whether this copy can install `release` itself: a copy the Windows or Linux
// setup installed, or the player's Android app. Any other copy, or a release
// that is not signed, is updated from the release's page.
[[nodiscard]] bool can_install(const update_rules::Release &release);

[[nodiscard]] Status status();

// Starts downloading on a thread of its own, unless that is under way.
void start(const update_rules::Release &release);
// Stops a download; what has arrived is kept for the next attempt.
void cancel();

// With the download Ready. Windows: starts the setup and closes the game.
// Android: hands the app to Android's installer, which asks the player and
// then stops the game to install it. Linux: puts the new program's files in
// place of this one's, closes the game and starts the new one as it exits
// (PortableKit's restart).
// False, with the status Failed, when that could not start.
bool install_now();

// At the program's start, before the game's code is loaded: true when the copy
// is new but its update never finished and the setup was started again, so
// this run should end (Windows). Also clears finished downloads, which is all
// it does on Android and Linux.
[[nodiscard]] bool finish_at_start();

// The profile's prepare_game in the Linux setup's build: after an update from
// the menu, compiles the game's code when the new version's differs, on the
// setup's screens, then clears what the update left. False when that failed
// and the game cannot start.
bool prepare_after_update(portablekit::install::InstallerUi *ui, const std::filesystem::path &executable,
                          const std::filesystem::path &disc_image);

} // namespace d012::update_install
