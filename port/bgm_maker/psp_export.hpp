#pragma once

// "Make a PSP / PPSSPP ISO" (Check & save, and BgmMaker --psp-iso): the
// battle music of every mod the game would load, added to a copy of the
// player's ISO (psp_iso.hpp). Each track is decoded as the game decodes it
// (44.1 kHz stereo, its loop as the game resolves it), encoded to ATRAC3 by
// atracdenc (a program of its own beside this one, LGPL-2.1, run once per
// track on worker threads) and named "<series short name> - <title>", since
// the stock selector has one flat list. MIT licence; see port/README.md.

#include "d012_mod_manifest.hpp"
#include "mods_folder.hpp"

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

namespace d012::bgm_maker {

struct PspTrack {
    std::string id;     // "<mod id>:<track key>"
    std::string name;   // as the game's list shows it: "FFXVI - Away"
    std::filesystem::path file;
    mod_manifest::Loop loop;
};

// The tracks the game would add from `mods`, in its order (host/d012_mods.cpp
// load(): mods by folder, on in enabled.json, series by order then id).
struct PspPlan {
    std::vector<PspTrack> tracks;
    std::vector<std::string> left_out;  // "<folder>: why" for each mod not in it
};
[[nodiscard]] PspPlan psp_plan(const std::filesystem::path &mods);

// What a series without "short" is called in the export: "FINAL FANTASY XVI"
// -> "FFXVI", else the name (cut to 16 bytes).
[[nodiscard]] std::string default_short_name(std::string_view series_name);
// "<short> - <title>".
[[nodiscard]] std::string psp_track_name(std::string_view short_name, std::string_view title);
// The longest name of the disc's own list (37 characters, "FFVIII - The Man with the Machine Gun"):
// longer ones may not fit the selector's box.
inline constexpr std::size_t kLongestDiscName = 37u;

// atracdenc beside this program (or, in a checkout, the one
// tools/port/build_atracdenc.cmd builds); empty when there is none.
[[nodiscard]] std::filesystem::path encoder_program(const ModsFolder &mods);

// The ISO the game was set up with (settings.ini's disc_image in the data
// folder the mods are in, as the installed game has it; game/disc.iso beside
// the program; or build/port/game/disc.iso of a checkout), as the default
// original; empty when there is none.
[[nodiscard]] std::filesystem::path game_iso(const ModsFolder &mods);

class PspExport {
public:
    enum class Phase { Idle, Checking, Encoding, Writing, Done, Failed, Cancelled };
    struct Status {
        Phase phase{Phase::Idle};
        std::uint64_t done{}, total{};  // of the phase: bytes read or written, tracks encoded
        std::string message;            // what failed, or what was made
    };

    PspExport() = default;
    ~PspExport();  // cancels and waits
    PspExport(const PspExport &) = delete;
    PspExport &operator=(const PspExport &) = delete;

    // Starts on a thread of its own; false with `error` when it cannot.
    bool start(const std::filesystem::path &original, const std::filesystem::path &out, const ModsFolder &mods,
               std::string &error);
    void cancel();
    [[nodiscard]] Status status();
    [[nodiscard]] bool busy();

private:
    void run(std::filesystem::path original, std::filesystem::path out, ModsFolder mods);
    void set_phase(Phase phase, std::uint64_t total);
    void finish(Phase phase, std::string message);

    std::mutex mutex_;
    Status status_;
    std::atomic<std::uint64_t> done_{0};
    std::atomic<bool> cancel_{false};
    std::atomic<bool> busy_{false};
    std::thread worker_;
};

} // namespace d012::bgm_maker
