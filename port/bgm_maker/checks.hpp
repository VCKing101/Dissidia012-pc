#pragma once

// Checking the mod being edited: the game's own rules (d012_mod_manifest.cpp)
// on the manifest the tool would save, then the tool's checks across mods.
// MIT licence; see port/README.md.

#include "d012_mod_manifest.hpp"
#include "model.hpp"
#include "mods_folder.hpp"

#include <filesystem>
#include <string_view>
#include <vector>

namespace d012::bgm_maker {

using Problem = mod_manifest::Problem;

// `folder`: where the mod is, or will be once saved. `others`: the other mods
// in the mods folder (not this one).
[[nodiscard]] std::vector<Problem> check_mod(const ModModel &model, const std::filesystem::path &folder,
                                             const std::vector<ModSummary> &others);

// The same for a manifest's text as it is on disc (`id`: its mod id, for the
// checks across mods); the problems keep their lines.
[[nodiscard]] std::vector<Problem> check_text(const std::string &text, const std::string &id,
                                              const std::filesystem::path &folder, const std::vector<ModSummary> &others);

// Why `name` cannot be a new mod's folder in `mods`; empty when it can.
[[nodiscard]] std::string folder_name_problem(std::string_view name, const std::filesystem::path &mods);

// The problems at `path` exactly, or below it ("mod" covers "mod.id").
[[nodiscard]] std::vector<const Problem *> problems_at(const std::vector<Problem> &problems, std::string_view path,
                                                       bool below = false);

} // namespace d012::bgm_maker
