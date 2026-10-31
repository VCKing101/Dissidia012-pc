#pragma once

// Checking the mod being edited: the game's own rules (d012_mod_manifest.cpp)
// on the manifest the tool would save, then the tool's checks across mods.
// MIT licence; see port/README.md.

#include "d012_mod_manifest.hpp"
#include "model.hpp"
#include "mods_folder.hpp"

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string_view>
#include <vector>

namespace d012::bgm_maker {

using Problem = mod_manifest::Problem;

// The port's limit on a track (PortableKit host_audio.cpp kMaxFrames).
inline constexpr std::int64_t kMaxTrackSeconds = 30 * 60;

// What the tool knows of a track's file, for the checks the game can only
// make once it plays it: whether FFmpeg reads it, and its length.
struct FileFacts {
    std::string error;                     // why it cannot be played; empty when it can
    int rate{};                            // the rate loop samples count in
    std::optional<std::int64_t> samples;   // exact, once decoded
    std::optional<std::int64_t> header_samples;  // a BRSTM, BCSTM or BFSTM header's length
    std::string header_format;             // "BRSTM"
};
// The facts of a track's file by its text in the manifest; none when not known (yet).
using FactsOf = std::function<std::optional<FileFacts>(const std::string &file)>;

// `folder`: where the mod's files are (its folder, or a new mod's files until
// it is saved). `others`: the other mods in the mods folder (not this one).
// The game's own rules (d012_mod_manifest.cpp), then the tool's, as the
// Python tool's validate(): ids and track numbers clashing with other mods
// (the game leaves one out), and with `facts`, files FFmpeg cannot play,
// longer than 30 minutes, loops outside the file; titles outside ASCII.
[[nodiscard]] std::vector<Problem> check_mod(const ModModel &model, const std::filesystem::path &folder,
                                             const std::vector<ModSummary> &others, const FactsOf &facts = {});

// The same for a manifest's text as it is on disc (`id`: its mod id, for the
// checks across mods); the problems keep their lines.
[[nodiscard]] std::vector<Problem> check_text(const std::string &text, const std::string &id,
                                              const std::filesystem::path &folder, const std::vector<ModSummary> &others,
                                              const FactsOf &facts = {});

// Why the port would not take a track's loop (and would loop the whole file
// instead); empty when it would. PortableKit host_audio.cpp resolve_loop.
[[nodiscard]] std::string loop_range_problem(const mod_manifest::Loop &loop, int rate, std::int64_t samples);

// Why `name` cannot be a new mod's folder in `mods`; empty when it can.
[[nodiscard]] std::string folder_name_problem(std::string_view name, const std::filesystem::path &mods);

// The problems at `path` exactly, or below it ("mod" covers "mod.id").
[[nodiscard]] std::vector<const Problem *> problems_at(const std::vector<Problem> &problems, std::string_view path,
                                                       bool below = false);

} // namespace d012::bgm_maker
