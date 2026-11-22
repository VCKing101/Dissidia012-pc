#pragma once

// A fighter's effect package ("FEP\xCD", the .fep member of an objx `effe`
// chunk): reading one, writing it back, and copying an effect from one
// package into another with everything it uses. This is
// tools/effect_package.py in C++, and writes the same bytes: online play
// needs every machine to build the same package, so integers are read and
// written a byte at a time and nothing is ordered by a hash or an address.
// port/tests/check_effect_package.cpp checks it against packages the Python
// wrote. Nothing here reads files, logs or needs PortableKit.
//
// A package is a graph of blocks joined by pointers, stored as offsets from
// the start of the file. After loading, the game turns every pointer into an
// address (main 0x0894351C). This finds the same pointers by walking the same
// structure, splits the file into blocks at their targets, and writes blocks
// back with the pointers recomputed. An unchanged package is written back
// byte for byte.
//
// Layout (from main 0x0894351C; field names partly from dissidia-fan's notes
// on the format):
//
//   header 0x48   +0x00 'FEP\xCD', u16 1, u16 3
//                 +0x08 ptr {u32 n, ptr descriptors}: n x 8 bytes {u8, u8, u16, u16 parameter
//                       size, u16 state size}, one per channel type (the same in every file)
//                 +0x0C runtime flags (0 in files), +0x10 package id base, +0x14 file size
//                 +0x1C u16 effects, +0x1E u16 bodies, +0x20 u16 x4 TEX, MDL, ANM, VTL counts
//                 +0x28 ptr effects[], +0x2C ptr bodies[], +0x30..+0x3C ptr TEX/MDL/ANM/VTL refs[]
//                 +0x40 ptr (if < 0x500000) {u16 n, ptr items[]}, item +8 ptr
//                 +0x44 ptr names {u32 n, ptr entries}: entries 12 bytes {u32 effect id, u32 length, ptr text}
//   effect  12    {u32 id, u16 body, u16 leaves, ptr leaves[]}; leaf +0x0C ptr (may be 0)
//                 id = package base | group << 10 | leaf bits; a package holds groups 0..127
//   body    52    {.., +0x10 u16 spawns, +0x12 u16 nodes, +0x18 u16 serials, +0x1A u16 p48,
//                 +0x1C ptr spawns[] (item +4 ptr), +0x20 ptr nodes[] (may hold 0),
//                 +0x24 ptr channels, +0x28 ptr p40[12] (item +4 ptr), +0x2C ptr serials[] (item +4 ptr),
//                 +0x30 ptr p48[]}
//   node    72    {+0x08 u8 tracks (0: nothing below is a pointer), +0x12 u8 has_init, +0x13 u8 segments,
//                 +0x1C ptr tracks[] (track 12 bytes {u16 channel, u16, u32 keys, ptr keys}),
//                 +0x20 ptr initial parameters (0 without has_init), +0x24 ptr segments[] (item +8 ptr),
//                 +0x28..+0x44 eight ptr lists}
//   parameters    {u16 frame0, u16 frame1, u32 n, ptr table}; table n x {u16 track, u16 offset, u16, u16},
//                 each track's parameter block at offset - 12 into the table block
//   resource 20   {f32, u32 id, u32 size, ptr data, i32 -1}; size 0 = a shared library resource
//
// Which parameter fields name a resource (by index into its table) is
// kResourceFields in the .cpp; models and texture animations name their
// textures by id. MIT licence; see port/README.md.

#include <cstddef>
#include <cstdint>
#include <deque>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace d012::effect_package {

// The data is not an effect package, or not as the layout above says.
struct Error : std::runtime_error {
    using std::runtime_error::runtime_error;
};

// A block is known by its number in its package; two blocks with the same
// bytes are still two blocks.
using BlockId = std::uint32_t;
inline constexpr BlockId kNoBlock = 0xFFFFFFFFu;

struct Pointer {
    std::uint64_t at{};         // offset in the block that holds it
    BlockId target{kNoBlock};   // the block it points at
};

// Bytes of the package from one pointer target to the next, with the
// pointers inside them.
struct Block {
    std::vector<std::uint8_t> data;
    std::vector<Pointer> pointers;  // in the order found or added; each `at` once
    std::uint64_t pos{};            // where it started in the file it came from; gives its alignment
};

// Where a copied effect went: its index in the name table and its new id.
struct Copied {
    std::size_t index{};
    std::uint32_t id{};
};

class Package {
public:
    explicit Package(std::span<const std::uint8_t> bytes);  // throws Error
    // The package as a file: each block where its alignment allows (what its
    // original offset gives, 16 at most, else 4), pointers and the file size
    // (+0x14) recomputed. Throws Error when a pointer cannot be written (a
    // package that a refused copy left half changed).
    [[nodiscard]] std::vector<std::uint8_t> to_bytes() const;
    // The effects' names in name-table order (an effect placement's +0x28 is
    // an index here). Latin-1 bytes, as the package holds them.
    [[nodiscard]] std::vector<std::string> names() const;
    // The effects' ids, in the same order.
    [[nodiscard]] std::vector<std::uint32_t> ids() const;

private:
    friend Copied copy_effect_with_id(Package &target, const Package &source, std::string_view name,
                                      std::string_view new_name);
    std::deque<Block> blocks_;  // in the order written; the first is the header
};

// Copies effect `name` of `source` into `target` as `new_name` (as `name`
// when that is empty), with its body, its nodes and the resources they use (a
// resource `target` already holds, byte for byte, is shared). Placement
// records (FEPA) name an effect by the index returned. `source` is only read;
// it may be `target` itself. Throws Error, and then `target` may be half
// changed: do not use it.
Copied copy_effect_with_id(Package &target, const Package &source, std::string_view name,
                           std::string_view new_name);
// The same, returning the copy's index in target's name table.
std::size_t copy_effect(Package &target, const Package &source, std::string_view name, std::string_view new_name);

} // namespace d012::effect_package
