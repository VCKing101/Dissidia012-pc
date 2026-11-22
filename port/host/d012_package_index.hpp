#pragma once

// The game's package: DATA/PACKAGE.BIN, the files of the game end to end, and
// DATA/PACKAGE_INFO.BIN, its index. This is the plan for serving a mod's
// files in place of package entries (d012_package_mods.cpp): the index the
// game is given instead, with the new sizes and places, and what the game
// reads at each offset of PACKAGE.BIN. Nothing here reads files, logs or
// needs PortableKit, so the tests (port/tests) check it with a made-up
// package. The format is docs/port/mod-folder.md, section 9. MIT licence;
// see port/README.md.

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace d012::package_index {

inline constexpr std::uint32_t kSector = 2048u;
inline constexpr std::size_t kHeaderBytes = 16u;
inline constexpr std::size_t kEntryBytes = 12u;
// An entry's size shares a word with its flags (flags << 28 | size).
inline constexpr std::uint32_t kMaxEntrySize = 0x0FFFFFFFu;

// The CRC an entry is found by: BZip2's CRC-32 (polynomial 0x04C11DB7, most
// significant bit first, starting from and finished with 0xFFFFFFFF) of the
// path in lower case, '/' between folders ("obj/p_sev100.objx").
[[nodiscard]] std::uint32_t path_crc(std::string_view path);

struct Entry {
    std::uint32_t crc{};
    std::uint32_t sector{};  // PACKAGE.BIN offset / kSector
    std::uint32_t size{};    // bytes
    std::uint32_t flags{};   // the top four bits of the size word, kept as they are
};

// PACKAGE_INFO.BIN: {u32 magic, "pack", u32 count, u32 0}, then `count`
// 12-byte entries {crc, sector, flags << 28 | size} sorted by CRC.
struct Index {
    std::uint32_t magic{};
    std::uint32_t reserved{};
    std::vector<Entry> entries;
};
[[nodiscard]] std::optional<Index> parse(std::span<const std::uint8_t> bytes, std::string &error);
[[nodiscard]] std::vector<std::uint8_t> write(const Index &index);
// One past the last sector any entry takes: where the package ends.
[[nodiscard]] std::uint64_t end_sector(const Index &index);
// The sectors `size` bytes take.
[[nodiscard]] constexpr std::uint64_t sectors(std::uint64_t size) { return (size + kSector - 1u) / kSector; }

// A mod's file in place of the entry with `crc`.
struct Replacement {
    std::uint32_t crc{};
    std::shared_ptr<const std::vector<std::uint8_t>> bytes;
    std::string name;  // for the log: "<mod id>/<path in its package folder>"
};

// A stretch of PACKAGE.BIN the game is served from a replacement: its bytes,
// then zeros to the end of the stretch (the rest of its last sector, or of
// the entry it replaces when it is smaller).
struct Region {
    std::uint64_t offset{};
    std::uint64_t length{};
    std::shared_ptr<const std::vector<std::uint8_t>> bytes;
    std::string name;
};

struct Plan {
    std::vector<std::uint8_t> index;  // PACKAGE_INFO.BIN as the game reads it
    std::vector<Region> regions;      // sorted by offset, never overlapping
    std::uint64_t package_size{};     // PACKAGE.BIN as the game sees it
    std::uint64_t relocated_bytes{};  // what moved past the end of the real file
    std::size_t in_place{};           // replacements that fit where the entry was
    std::size_t relocated{};          // replacements bigger than that
    // CRCs the index has no entry for, and replacements too big for the
    // size word; left out.
    std::vector<std::uint32_t> missing;
    std::vector<std::uint32_t> too_big;
};

// A replacement that fits in the sectors its entry takes stays there; a
// bigger one is given new sectors past the end of the package, in the order
// given, so the real file's other entries are never touched. Each CRC once:
// a later replacement of the same CRC is ignored (the caller has said which
// one wins). `package_size` is the real PACKAGE.BIN's size.
[[nodiscard]] Plan plan(const Index &index, std::uint64_t package_size, const std::vector<Replacement> &replacements);

// What the game reads at `offset` of PACKAGE.BIN under `plan`: a region's
// bytes, or the real file's (`read_real`, which returns how many bytes it
// gave, fewer at its end), or zeros between the real file's end and the
// plan's. Returns how many bytes were written to `destination`.
using RealReader = std::function<std::size_t(std::uint64_t offset, std::uint8_t *destination, std::size_t length)>;
std::size_t read(const Plan &plan, std::uint64_t offset, std::uint8_t *destination, std::size_t length,
                 const RealReader &read_real);

} // namespace d012::package_index
