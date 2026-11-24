#pragma once

// The mods folder the Battle BGM Maker works on, found the way the game finds
// its own (host/d012_mods.cpp mods_directory, host/d012_portable.cpp), and the
// mods in it. MIT licence; see port/README.md.

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace d012::bgm_maker {

struct ModsFolder {
    std::filesystem::path path;
    std::string found_by;  // for the window: "D012_MODS_DIR", "the game's data folder", ...
    // How the game, started from here, finds these mods: on its own (the
    // folder it would find), or told with D012_MODS_DIR (`tell_game`); in a
    // checkout, as play_d012.ps1 starts it (`checkout`: the checkout's root).
    bool tell_game{};
    std::filesystem::path checkout;
    // Picked among several copies of the game found on this computer: the
    // window asks which one.
    bool ask{};
};

// A copy of the game found on this computer, with its data folder.
struct GameCopy {
    std::filesystem::path folder;
    bool windows{};               // the Windows version (D012Native.exe, under Proton)
    std::int64_t last_played{};   // seconds since 1970 of its newest log, settings or save; 0: never
};

// On Linux, the copies of the game in the places a player's are: the folder
// the Linux setup's menu entry names, Steam's shortcuts to D012Native (read,
// never changed) and the folders in ~/Games; each with D012Native (or the
// Windows version's D012Native.exe) and a data/ folder, the one played last
// first. Elsewhere none: the tool is installed beside the game.
[[nodiscard]] std::vector<GameCopy> find_game_copies();

// In order: `chosen` (the command line), D012_MODS_DIR, D012_DATA_DIR/mods,
// the data/ folder beside the program (in the installer build when the game
// is there too, else when game/EBOOT.ELF is), build/port/data/mods of the
// checkout a build directory is in, the folder last opened in the window,
// on Linux the copy of the game played last (find_game_copies), and the
// game's per-user data folder.
[[nodiscard]] ModsFolder find_mods_folder(const std::optional<std::filesystem::path> &chosen);

// The folder this program is in, as the player sees it: an AppImage's own
// folder when it runs from one (Linux), not the folder it is unpacked to.
[[nodiscard]] std::filesystem::path tool_folder();

// Whether this program runs from an AppImage.
[[nodiscard]] bool from_appimage();

// The game's program (D012Native) in `folder`; empty when it is not there.
[[nodiscard]] std::filesystem::path game_in(const std::filesystem::path &folder);

// Remembers `folder`, opened in the window, for the next start
// (find_mods_folder).
void remember_mods_folder(const std::filesystem::path &folder);

// One mod folder, read quickly: what the list shows and what the clash checks
// across mods need.
struct ModSummary {
    std::string folder;  // its name in the mods folder
    std::string id;
    std::string name;
    std::string version;
    std::string error;  // why its mod.json cannot be read; empty when it can
    std::map<std::uint32_t, std::string> tracks;  // track number -> track id
};

// Every folder in `mods`, by name (hidden ones, starting with '.', left out).
[[nodiscard]] std::vector<ModSummary> list_mods(const std::filesystem::path &mods);

} // namespace d012::bgm_maker
