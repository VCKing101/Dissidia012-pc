#pragma once

// DLC packs: what the game is served from <data dir>/dlc/<pack>/. The game
// finds DLC by the hash of a name made from slot numbers, so two packs built
// for the same slot would be the same files in the memory stick's one folder.
// Kept in a folder each, a pack that wants a slot already taken is given a free
// one: its tables are rewritten and its files served under the new names. This
// is the plan for that. Nothing here reads files, logs or needs PortableKit, so
// the tests (port/tests) check it with made-up packs. The player's guide is
// docs/port/dlc-packs.md. MIT licence; see port/README.md.

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace d012::dlc_packs {

// The hash the game finds a DLC file by: BZip2's CRC-32 of the path in lower
// case ("dlc/obj/dlc_001oe.bin" is eac7bf53.edat).
[[nodiscard]] std::uint32_t name_crc(std::string_view game_path);
// The hash a file's name spells: "e2734686.edat", in any case; empty for any
// other name.
[[nodiscard]] std::optional<std::uint32_t> crc_of_file_name(std::string_view file_name);

struct SourceFile {
    std::uint32_t crc{};  // the hash its name spells
    std::uint64_t size{};
    std::filesystem::path path;
};

struct Pack {
    std::string name;  // for the log
    std::vector<SourceFile> files;
};

using ReadFile = std::function<std::vector<std::uint8_t>(const SourceFile &)>;

// A file the game is given at the name `crc`: a pack's file as it is, or, with
// `rewritten`, a table with new slots.
struct Served {
    std::uint32_t crc{};
    std::filesystem::path path;
    std::uint64_t size{};
    bool rewritten{};
    std::vector<std::uint8_t> bytes;
    std::string pack;
};

struct Plan {
    std::vector<Served> files;          // sorted by crc, each crc once
    std::vector<std::string> moved;     // what was given another slot
    std::vector<std::string> problems;  // what was left out, and why
};

// `packs` are taken in the order given; what the memory stick's own folder
// holds (`installed`) keeps its slots. Each pack keeps the slots it was built
// for when they are free, so a pack on its own is served as it was made.
[[nodiscard]] Plan plan(const std::vector<Pack> &packs, const Pack &installed, const ReadFile &read);

} // namespace d012::dlc_packs
