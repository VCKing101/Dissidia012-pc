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
};

// In order: `chosen` (the command line), D012_MODS_DIR, D012_DATA_DIR/mods,
// the data/ folder beside the program (always in the installer build, else
// when game/EBOOT.ELF is there too), build/port/data/mods of the checkout a
// build directory is in, and the game's per-user data folder.
[[nodiscard]] ModsFolder find_mods_folder(const std::optional<std::filesystem::path> &chosen);

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
