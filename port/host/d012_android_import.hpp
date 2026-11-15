#pragma once

// Adding content on Android (d012_android_import.cpp). An Android app reads
// only its own storage, and the files a player downloads are in shared
// storage (Downloads, an SD card) that it can open only through Android's
// file picker, as content:// documents. So mods, DLC and the Japanese disc
// are picked there and copied into the app's own folders, where the port
// reads them as on any other system:
//
// - System, "Add content": "Import mods" and "Import DLC" pick a folder;
// - Options, "Japanese Voice Disc" (d012_voices_menu.cpp) picks the image and
//   copies it with copy_document().
//
// Everything here is the Android app's only (PORTABLEKIT_ANDROID_APP);
// elsewhere install() does nothing. MIT licence; see port/README.md.

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

namespace portablekit {
class HleRegistrar;
}

namespace d012::android_import {

// A copy's progress, read by the interface while a worker thread copies.
struct Progress {
    std::atomic<std::uint64_t> done{0};
    std::atomic<std::uint64_t> total{0};
    std::atomic<bool> cancel{false};
};

// Copies the content:// document `uri` to `target`, through <target>.part,
// adding its bytes to `progress` (its total is set when only one file is
// copied, `set_total`). Nothing on success, else the reason for the player.
// Checks for free space first.
[[nodiscard]] std::optional<std::string> copy_document(const std::string &uri, const std::filesystem::path &target,
                                                       Progress &progress, bool set_total = true);

// Adds the System page's "Add content" rows in the Android app.
void install(portablekit::HleRegistrar &hle);

} // namespace d012::android_import
