#pragma once

// Package mods: files a mod gives in place of entries of the game's
// PACKAGE.BIN (a character's moveset, obj/p_sev100.objx, or its model), as
// dissidia-fan's charmod.py writes them. A mod names its package folder in
// mod.json ("package"); the folder holds each file at its game path, or as
// by_crc/<CRC, 8 hex digits>.bin for an entry whose path is not known. The
// game is given an index (PACKAGE_INFO.BIN) with the new sizes and places,
// and its reads of PACKAGE.BIN inside a replaced entry are served from the
// mod's file; the files on the disc are never changed. Format and rules:
// docs/port/mod-folder.md, section 9. Off with D012_NO_PACKAGE_MODS=1. MIT
// licence; see port/README.md.

#include "portablekit/hle_extension.hpp"

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace d012::package_mods {

struct Source {
    std::string mod_id;
    std::filesystem::path folder;  // the mod's package folder
};

// Files a part of the port makes itself (Bartz's original moves,
// d012_original_moves_feature.hpp) rather than reads from a mod's folder.
// They are asked for when the game first opens its package, which is when the
// disc can be read. `taken` tells whether a mod already replaces the entry
// with that CRC (package_index::path_crc): a mod's file wins, and a source
// whose files belong together gives all of them or none.
struct LateFile {
    // Package path, as in a mod's package folder; by_crc/<CRC>.bin for an
    // entry whose path is not known.
    std::string path;
    std::shared_ptr<const std::vector<std::uint8_t>> bytes;
};
using LateSource = std::function<std::vector<LateFile>(const std::function<bool(std::uint32_t crc)> &taken)>;
// Before install(). `name` is for the log.
void add_late_source(std::string name, LateSource source);

// Reads the sources' files and, when any can be used, serves them to the game
// from its first open of the package. Sources are taken in the order given:
// when two replace the same entry, the first one does. Called once, by
// mods::install.
void install(const std::vector<Source> &sources);

// The Japanese voices (d012_voices.cpp) serve PACKAGE_INFO.BIN and
// PACKAGE.BIN themselves, and only one file provider serves a file; they
// pass their versions through these, so the mods' files go over them. The
// index the game reads, given the one it would read otherwise and the size
// of the package behind it (`base` itself when no mod file is used):
[[nodiscard]] std::vector<std::uint8_t> index_over(const std::vector<std::uint8_t> &base, std::uint64_t base_size);
// The package the game reads, over `base` (`base` itself when no mod file is
// used). index_over() comes first: it makes the plan.
[[nodiscard]] std::shared_ptr<portablekit::hle_extension::FileFilter>
package_over(std::shared_ptr<portablekit::hle_extension::FileFilter> base);

} // namespace d012::package_mods
