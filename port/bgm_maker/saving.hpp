#pragma once

// Saving a mod: checked first (checks.hpp), nothing written while an error is
// left; the mod.json it replaces kept in the mod's .backups/ (the last 20);
// for a new mod, its folder made and the files it waited with moved in. As
// the Python tool's save (port/tools/bgm_maker/bgmmaker/server.py), so the
// two write the same file. Used by the window and by the command line.
// MIT licence; see port/README.md.

#include "checks.hpp"
#include "model.hpp"
#include "mods_folder.hpp"

#include <filesystem>
#include <string>
#include <vector>

namespace d012::bgm_maker {

inline constexpr std::size_t kKeptBackups = 20;

struct SaveRequest {
    std::filesystem::path mods;    // the mods folder
    std::string folder;            // the mod's folder in it
    bool is_new{};                 // the folder is made (and must not be there yet)
    std::filesystem::path staging; // a new mod's files until now; empty when none
    ModModel model;
    FactsOf facts;                 // every track's file, decoded: exact lengths
    std::vector<ModSummary> others;
    // Files copied in while the mod was open (in the staging folder for a new
    // mod): those the saved manifest does not use are deleted once it is saved.
    std::vector<std::filesystem::path> added;
};

struct SaveResult {
    bool saved{};       // mod.json is as the model says (written, or already so)
    bool unchanged{};   // it was so already: nothing written
    std::vector<Problem> problems;
    std::string backup;  // the old mod.json's copy, relative to the mod (".backups/mod-....json"); empty when none
    std::string error;   // a file that could not be written; empty when none
    std::string text;    // mod.json's text
    std::vector<std::filesystem::path> deleted;  // of `added`
};

[[nodiscard]] SaveResult save_mod(const SaveRequest &request);

// Whether a list of problems has an error.
[[nodiscard]] bool has_errors(const std::vector<Problem> &problems);

} // namespace d012::bgm_maker
