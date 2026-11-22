// The effect package, read, written and added to; see
// d012_effect_package.hpp. Each part here is the part of the same name in
// tools/effect_package.py and does its steps in the same order, so the two
// write the same bytes, and leave a package the same way when they refuse
// one. Where the Python fails on a read past the end, a missing pointer or a
// number too big for its field, this throws Error. MIT licence; see
// port/README.md.

#include "d012_effect_package.hpp"

#include <algorithm>
#include <array>
#include <initializer_list>
#include <map>
#include <optional>
#include <set>

namespace d012::effect_package {
namespace {

using Bytes = std::span<const std::uint8_t>;
using Blocks = std::deque<Block>;

// The four resource tables, in the header's order.
constexpr std::size_t kTex = 0u;
constexpr std::size_t kMdl = 1u;
constexpr std::size_t kAnm = 2u;
constexpr std::size_t kVtl = 3u;
constexpr std::size_t kKinds = 4u;

// The parameter fields that hold an index into a resource table: the
// channel, the table, the byte offset in the channel's parameter block and
// the field's size (4: i32, 2: i16). From dissidia-fan's analysis of the
// channels' component code (their dfx/fep.py LINKS), checked against every
// package of both games (all indices in range). A channel's fields are in
// the order the Python lists them.
struct ResourceField {
    std::uint16_t channel;
    std::size_t kind;
    std::uint32_t field;
    std::uint32_t width;
};
constexpr ResourceField kResourceFields[] = {
    {10, kAnm, 12, 4},  {11, kMdl, 12, 4},  {20, kMdl, 12, 4},  {21, kAnm, 12, 4},  {21, kMdl, 244, 2},
    {21, kVtl, 246, 2}, {26, kMdl, 14, 2},  {26, kVtl, 16, 2},  {30, kMdl, 14, 2},  {30, kVtl, 16, 2},
    {150, kMdl, 14, 2}, {150, kVtl, 16, 2}, {53, kAnm, 16, 4},  {54, kAnm, 160, 4}, {55, kAnm, 16, 4},
    {56, kAnm, 16, 4},  {57, kAnm, 16, 4},  {63, kTex, 0, 4},   {78, kMdl, 4, 2},   {78, kVtl, 6, 2},
    {80, kAnm, 44, 4},  {105, kTex, 44, 4}, {108, kMdl, 16, 2}, {108, kVtl, 18, 2}, {119, kMdl, 12, 4},
};

[[noreturn]] void fail(const std::string &what) { throw Error(what); }

std::string hex(std::uint64_t value) {
    std::string text;
    do {
        text.insert(text.begin(), "0123456789abcdef"[value & 15u]);
        value >>= 4u;
    } while (value != 0u);
    return "0x" + text;
}

// Integers are little-endian. Every read and write checks its place first.
void need(std::size_t size, std::uint64_t at, std::size_t width) {
    if (size < width || at > size - width) fail("a field at " + hex(at) + " is past the end of its data");
}

std::uint8_t load8(Bytes bytes, std::uint64_t at) {
    need(bytes.size(), at, 1u);
    return bytes[static_cast<std::size_t>(at)];
}

std::uint16_t load16(Bytes bytes, std::uint64_t at) {
    need(bytes.size(), at, 2u);
    const std::size_t i = static_cast<std::size_t>(at);
    return static_cast<std::uint16_t>(bytes[i] | bytes[i + 1u] << 8u);
}

std::uint32_t load32(Bytes bytes, std::uint64_t at) {
    need(bytes.size(), at, 4u);
    const std::size_t i = static_cast<std::size_t>(at);
    return static_cast<std::uint32_t>(bytes[i]) | static_cast<std::uint32_t>(bytes[i + 1u]) << 8u |
           static_cast<std::uint32_t>(bytes[i + 2u]) << 16u | static_cast<std::uint32_t>(bytes[i + 3u]) << 24u;
}

void store16(std::vector<std::uint8_t> &bytes, std::uint64_t at, std::uint64_t value) {
    need(bytes.size(), at, 2u);
    if (value > 0xFFFFu) fail("a number does not fit its 16-bit field");
    const std::size_t i = static_cast<std::size_t>(at);
    bytes[i] = static_cast<std::uint8_t>(value & 0xFFu);
    bytes[i + 1u] = static_cast<std::uint8_t>(value >> 8u & 0xFFu);
}

void store32(std::vector<std::uint8_t> &bytes, std::uint64_t at, std::uint64_t value) {
    need(bytes.size(), at, 4u);
    if (value > 0xFFFFFFFFu) fail("a number does not fit its 32-bit field");
    const std::size_t i = static_cast<std::size_t>(at);
    for (std::size_t k = 0; k < 4u; ++k) bytes[i + k] = static_cast<std::uint8_t>(value >> (8u * k) & 0xFFu);
}

// Where a parameter field is in its table block. Python's struct takes an
// offset below zero as counted from the end, when the whole field is there.
std::uint64_t field_place(std::size_t size, std::int64_t at, std::uint32_t width) {
    if (at < 0) {
        if (at + static_cast<std::int64_t>(width) > 0 || static_cast<std::uint64_t>(-at) > size)
            fail("a parameter field is outside its table");
        return size - static_cast<std::uint64_t>(-at);
    }
    need(size, static_cast<std::uint64_t>(at), width);
    return static_cast<std::uint64_t>(at);
}

// A resource index as a parameter holds it: i32 or i16, below zero for none.
std::int64_t load_field(Bytes bytes, std::int64_t at, std::uint32_t width) {
    const std::uint64_t place = field_place(bytes.size(), at, width);
    if (width == 2u) {
        const std::uint16_t value = load16(bytes, place);
        return value >= 0x8000u ? static_cast<std::int64_t>(value) - 0x10000 : static_cast<std::int64_t>(value);
    }
    const std::uint32_t value = load32(bytes, place);
    return value >= 0x80000000u ? static_cast<std::int64_t>(value) - 0x100000000 : static_cast<std::int64_t>(value);
}

void store_field(std::vector<std::uint8_t> &bytes, std::int64_t at, std::uint32_t width, std::uint32_t value) {
    const std::uint64_t place = field_place(bytes.size(), at, width);
    if (value > (width == 2u ? 0x7FFFu : 0x7FFFFFFFu)) fail("a resource index does not fit its parameter field");
    if (width == 2u) store16(bytes, place, value);
    else store32(bytes, place, value);
}

// The block a block's pointer at `at` points to; kNoBlock without one.
BlockId target_at(const Block &block, std::uint64_t at) {
    for (const Pointer &pointer : block.pointers)
        if (pointer.at == at) return pointer.target;
    return kNoBlock;
}

const Block &block_at(const Blocks &blocks, BlockId id) {
    if (id >= blocks.size()) fail("the package lacks a pointer its layout needs");
    return blocks[id];
}

Block &block_at(Blocks &blocks, BlockId id) {
    if (id >= blocks.size()) fail("the package lacks a pointer its layout needs");
    return blocks[id];
}

const Block &follow(const Blocks &blocks, const Block &block, std::uint64_t at) {
    return block_at(blocks, target_at(block, at));
}

// Sets the pointer at `at`: in place when the block has one there, else
// after the others (a Python dict's order).
void set_pointer(Block &block, std::uint64_t at, BlockId target) {
    for (Pointer &pointer : block.pointers) {
        if (pointer.at == at) {
            pointer.target = target;
            return;
        }
    }
    block.pointers.push_back({at, target});
}

struct Slot {
    std::uint64_t at;      // offset of the pointer in the file
    std::uint64_t target;  // the offset it holds
};

// Every pointer the game relocates (main 0x0894351C), in the order its walk
// first meets them.
std::vector<Slot> pointer_slots(Bytes data) {
    std::vector<Slot> slots;
    std::vector<bool> seen(data.size());
    const std::uint64_t size = data.size();
    const auto ptr = [&](std::uint64_t slot, bool allow_zero = false) -> std::uint64_t {
        const std::uint32_t value = load32(data, slot);
        if (value == 0u && allow_zero) return 0u;
        if (value == 0u || value > size) fail("pointer " + hex(value) + " at " + hex(slot) + " is outside the package");
        if (!seen[static_cast<std::size_t>(slot)]) {
            seen[static_cast<std::size_t>(slot)] = true;
            slots.push_back({slot, value});
        }
        return value;
    };

    if (data.size() < 4u || data[0] != 'F' || data[1] != 'E' || data[2] != 'P' || data[3] != 0xCDu)
        fail("not an effect package (FEP)");
    std::uint64_t at = ptr(0x08u, true);
    if (at != 0u) ptr(at + 4u);
    at = ptr(0x44u, true);
    if (at != 0u) {
        const std::uint64_t entries = ptr(at + 4u);
        const std::uint32_t count = load32(data, at);
        for (std::uint64_t i = 0; i < count; ++i) ptr(entries + 12u * i + 8u);
    }
    if (const std::uint32_t value = load32(data, 0x40u); value > 0u && value < 0x500000u) {
        at = ptr(0x40u);
        const std::uint64_t items = ptr(at + 4u);
        const std::uint16_t count = load16(data, at);
        for (std::uint64_t i = 0; i < count; ++i) {
            const std::uint64_t item = ptr(items + 4u * i);
            ptr(item + 8u);
        }
    }
    for (std::uint64_t k = 0; k < kKinds; ++k) {
        const std::uint64_t table = ptr(0x30u + 4u * k, true);
        if (table == 0u) continue;
        const std::uint16_t count = load16(data, 0x20u + 2u * k);
        for (std::uint64_t i = 0; i < count; ++i) {
            const std::uint64_t ref = ptr(table + 4u * i);
            ptr(ref + 12u);
        }
    }
    std::uint64_t table = ptr(0x28u);
    const std::uint16_t effects = load16(data, 0x1Cu);
    for (std::uint64_t i = 0; i < effects; ++i) {
        const std::uint64_t effect = ptr(table + 4u * i);
        const std::uint64_t leaves = ptr(effect + 8u);
        const std::uint16_t count = load16(data, effect + 6u);
        for (std::uint64_t j = 0; j < count; ++j) {
            const std::uint64_t leaf = ptr(leaves + 4u * j);
            ptr(leaf + 12u, true);
        }
    }
    table = ptr(0x2Cu);
    const std::uint16_t bodies = load16(data, 0x1Eu);
    for (std::uint64_t i = 0; i < bodies; ++i) {
        const std::uint64_t body = ptr(table + 4u * i);
        const std::uint64_t spawns = ptr(body + 0x1Cu);
        const std::uint16_t spawn_count = load16(data, body + 0x10u);
        for (std::uint64_t j = 0; j < spawn_count; ++j) {
            const std::uint64_t item = ptr(spawns + 4u * j);
            ptr(item + 4u);
        }
        const std::uint64_t nodes = ptr(body + 0x20u);
        const std::uint16_t node_count = load16(data, body + 0x12u);
        for (std::uint64_t j = 0; j < node_count; ++j) {
            const std::uint64_t node = ptr(nodes + 4u * j, true);
            if (node == 0u || load8(data, node + 8u) == 0u) continue;
            const std::uint64_t tracks = ptr(node + 0x1Cu);
            const std::uint8_t track_count = load8(data, node + 8u);
            for (std::uint64_t k = 0; k < track_count; ++k) {
                const std::uint64_t track = ptr(tracks + 4u * k);
                ptr(track + 8u);
            }
            if (load8(data, node + 0x12u) != 0u) {
                const std::uint64_t init = ptr(node + 0x20u);
                ptr(init + 8u);
            }
            const std::uint64_t segments = ptr(node + 0x24u);
            const std::uint8_t segment_count = load8(data, node + 0x13u);
            for (std::uint64_t k = 0; k < segment_count; ++k) {
                const std::uint64_t segment = ptr(segments + 4u * k);
                ptr(segment + 8u);
            }
            for (std::uint64_t k = 0; k < 8u; ++k) ptr(node + 0x28u + 4u * k);
        }
        ptr(body + 0x24u);
        const std::uint64_t p40 = ptr(body + 0x28u);
        for (std::uint64_t j = 0; j < 12u; ++j) {
            const std::uint64_t item = ptr(p40 + 4u * j);
            ptr(item + 4u);
        }
        const std::uint64_t serials = ptr(body + 0x2Cu);
        const std::uint16_t serial_count = load16(data, body + 0x18u);
        for (std::uint64_t j = 0; j < serial_count; ++j) {
            const std::uint64_t item = ptr(serials + 4u * j);
            ptr(item + 4u);
        }
        const std::uint64_t p48 = ptr(body + 0x30u);
        const std::uint16_t p48_count = load16(data, body + 0x1Au);
        for (std::uint64_t j = 0; j < p48_count; ++j) ptr(p48 + 4u * j);
    }
    return slots;
}

// Where a block may start: what its original offset gives (16 at most), else 4.
std::uint64_t block_align(std::uint64_t pos) {
    for (const std::uint64_t align : {16u, 8u, 4u})
        if (pos % align == 0u) return align;
    return 4u;
}

// -- reading

// A count in the header.
std::uint16_t header_count(const Blocks &blocks, std::uint64_t at) { return load16(blocks.front().data, at); }

// The first `count` pointers of the header's table at `at` (kNoBlock where
// the table has none); nothing without the table.
std::vector<BlockId> header_table(const Blocks &blocks, std::uint64_t at, std::uint16_t count) {
    std::vector<BlockId> out;
    const BlockId id = target_at(blocks.front(), at);
    if (id == kNoBlock) return out;
    const Block &block = block_at(blocks, id);
    out.reserve(count);
    for (std::uint64_t i = 0; i < count; ++i) out.push_back(target_at(block, 4u * i));
    return out;
}

std::vector<BlockId> effects(const Blocks &blocks) {
    return header_table(blocks, 0x28u, header_count(blocks, 0x1Cu));
}

std::vector<BlockId> bodies(const Blocks &blocks) {
    return header_table(blocks, 0x2Cu, header_count(blocks, 0x1Eu));
}

std::vector<BlockId> refs(const Blocks &blocks, std::size_t kind) {
    return header_table(blocks, 0x30u + 4u * kind, header_count(blocks, 0x20u + 2u * kind));
}

struct Name {
    std::uint32_t id;
    std::string name;
};

// The name table: each effect's id and name, in its order.
std::vector<Name> name_table(const Blocks &blocks) {
    const Block &head = follow(blocks, blocks.front(), 0x44u);
    const Block &entries = follow(blocks, head, 4u);
    std::vector<Name> out;
    const std::uint32_t count = load32(head.data, 0u);
    for (std::uint64_t i = 0; i < count; ++i) {
        const std::vector<std::uint8_t> &text = follow(blocks, entries, 12u * i + 8u).data;
        const std::uint32_t id = load32(entries.data, 12u * i);
        const auto end = std::find(text.begin(), text.end(), std::uint8_t{0});
        if (end == text.end()) fail("an effect's name does not end");
        out.push_back({id, std::string(reinterpret_cast<const char *>(text.data()),
                                       static_cast<std::size_t>(end - text.begin()))});
    }
    return out;
}

// Each channel type's parameter size (its descriptor's third field).
std::vector<std::uint16_t> parameter_sizes(const Blocks &blocks) {
    const Block &head = follow(blocks, blocks.front(), 0x08u);
    const Block &table = follow(blocks, head, 4u);
    std::vector<std::uint16_t> out;
    const std::uint32_t count = load32(head.data, 0u);
    for (std::uint64_t i = 0; i < count; ++i) {
        need(table.data.size(), 8u * i, 8u);
        out.push_back(load16(table.data, 8u * i + 4u));
    }
    return out;
}

std::uint32_t base(const Blocks &blocks) { return load32(blocks.front().data, 0x10u); }

struct Field {
    BlockId table;         // the parameter table block that holds it
    std::int64_t at;       // its offset there
    std::size_t kind;      // the resource table it indexes
    std::uint32_t width;   // 4: i32, 2: i16
};

// Every resource index in a body's nodes' parameters.
std::vector<Field> parameter_fields(const Blocks &blocks, BlockId body_id) {
    const std::vector<std::uint16_t> sizes = parameter_sizes(blocks);
    std::vector<Field> out;
    const Block &body = block_at(blocks, body_id);
    const BlockId nodes = target_at(body, 0x20u);
    const std::uint16_t node_count = load16(body.data, 0x12u);
    for (std::uint64_t j = 0; j < node_count; ++j) {
        const BlockId node_id = nodes != kNoBlock ? target_at(block_at(blocks, nodes), 4u * j) : kNoBlock;
        if (node_id == kNoBlock) continue;
        const Block &node = block_at(blocks, node_id);
        const std::uint8_t track_count = load8(node.data, 8u);
        if (track_count == 0u) continue;
        const BlockId tracks = target_at(node, 0x1Cu);
        std::vector<std::uint16_t> channels;
        for (std::uint64_t k = 0; k < track_count; ++k)
            channels.push_back(load16(follow(blocks, block_at(blocks, tracks), 4u * k).data, 0u));
        // The parameter sets: the initial one, then one per segment.
        std::vector<BlockId> sets;
        if (load8(node.data, 0x12u) != 0u) sets.push_back(target_at(node, 0x20u));
        const BlockId segments = target_at(node, 0x24u);
        const std::uint8_t segment_count = load8(node.data, 0x13u);
        for (std::uint64_t k = 0; k < segment_count; ++k) sets.push_back(target_at(block_at(blocks, segments), 4u * k));
        for (const BlockId set : sets) {
            const Block &params = block_at(blocks, set);
            const BlockId table_id = target_at(params, 8u);
            const std::uint32_t entries = load32(params.data, 4u);
            for (std::uint64_t q = 0; q < entries; ++q) {
                const Block &table = block_at(blocks, table_id);
                need(table.data.size(), 8u * q, 4u);
                const std::uint16_t track = load16(table.data, 8u * q);
                const std::uint16_t offset = load16(table.data, 8u * q + 2u);
                if (track >= channels.size()) fail("a parameter names a track its node lacks");
                const std::uint16_t channel = channels[track];
                for (const ResourceField &field : kResourceFields) {
                    if (field.channel != channel) continue;
                    const std::int64_t at = std::int64_t{offset} - 12 + std::int64_t{field.field};
                    if (channel >= sizes.size()) fail("a track names a channel type the package lacks");
                    if (field.field + field.width <= sizes[channel] &&
                        at + static_cast<std::int64_t>(field.width) <= static_cast<std::int64_t>(table.data.size()))
                        out.push_back({table_id, at, field.kind, field.width});
                }
            }
        }
    }
    return out;
}

// The data of a resource reference; nothing for a shared library resource.
std::optional<Bytes> resource_bytes(const Blocks &blocks, const Block &ref) {
    const std::uint32_t size = load32(ref.data, 8u);
    if (size == 0u) return std::nullopt;
    const std::vector<std::uint8_t> &data = follow(blocks, ref, 12u).data;
    return Bytes(data.data(), std::min<std::size_t>(size, data.size()));
}

// The texture ids inside a model (MDL materials, 24 bytes from +0x10's
// offset, count at +0x0C) or a texture animation (ANM: count at +0x20,
// offset at +0x30): `count` of them, `step` apart, from offset `first`.
struct IdFields {
    std::uint64_t first{};
    std::uint64_t step{};
    std::uint64_t count{};
};

IdFields texture_id_fields(std::size_t kind, Bytes data) {
    const auto tagged = [&](char a, char b, char c) {
        return data.size() >= 4u && data[0] == a && data[1] == b && data[2] == c && data[3] == 0u;
    };
    if (kind == kMdl && tagged('M', 'D', 'L')) return {load32(data, 0x10u), 24u, load32(data, 0x0Cu)};
    if (kind == kAnm && tagged('A', 'N', 'M')) return {load32(data, 0x30u), 4u, load32(data, 0x20u)};
    return {};
}

using Used = std::array<std::set<std::uint32_t>, kKinds>;

// The indices, by table, a body's nodes name; textures of its models and
// animations included.
Used resources_of(const Blocks &blocks, BlockId body) {
    Used used;
    for (const Field &field : parameter_fields(blocks, body)) {
        const std::int64_t value = load_field(block_at(blocks, field.table).data, field.at, field.width);
        if (value >= 0) used[field.kind].insert(static_cast<std::uint32_t>(value));
    }
    // A texture id held twice is the later one of the table.
    std::map<std::uint32_t, std::uint32_t> by_id;
    const std::vector<BlockId> textures = refs(blocks, kTex);
    for (std::size_t i = 0; i < textures.size(); ++i)
        by_id[load32(block_at(blocks, textures[i]).data, 4u)] = static_cast<std::uint32_t>(i);
    for (const std::size_t kind : {kMdl, kAnm}) {
        const std::vector<BlockId> kind_refs = refs(blocks, kind);
        for (const std::uint32_t index : used[kind]) {
            if (index >= kind_refs.size()) fail("a parameter names a resource the package lacks");
            const std::optional<Bytes> data = resource_bytes(blocks, block_at(blocks, kind_refs[index]));
            if (!data) continue;
            const IdFields fields = texture_id_fields(kind, *data);
            for (std::uint64_t i = 0; i < fields.count; ++i) {
                const auto found = by_id.find(load32(*data, fields.first + fields.step * i));
                if (found != by_id.end()) used[kTex].insert(found->second);
            }
        }
    }
    return used;
}

// Every block a block leads to through its pointers, itself first: depth
// first, a block's pointers in the order of their offsets.
std::vector<BlockId> reachable(const Blocks &blocks, BlockId start) {
    std::vector<bool> seen(blocks.size());
    std::vector<BlockId> order;
    std::vector<BlockId> stack{start};
    while (!stack.empty()) {
        const BlockId current = stack.back();
        stack.pop_back();
        const Block &block = block_at(blocks, current);
        if (seen[current]) continue;
        seen[current] = true;
        order.push_back(current);
        std::vector<Pointer> pointers = block.pointers;
        std::sort(pointers.begin(), pointers.end(), [](const Pointer &a, const Pointer &b) { return a.at > b.at; });
        for (const Pointer &pointer : pointers) stack.push_back(pointer.target);
    }
    return order;
}

// Copies of some blocks of a package, numbered as they will be once they are
// added to a package that holds `first` blocks.
struct Clones {
    std::vector<Block> blocks;
    std::vector<BlockId> place;  // by the original's number: its copy's place in `blocks`
    BlockId first{};

    [[nodiscard]] std::size_t place_of(BlockId original) const {
        if (original >= place.size() || place[original] == kNoBlock) fail("a pointer leaves the blocks being copied");
        return place[original];
    }
    [[nodiscard]] Block &of(BlockId original) { return blocks[place_of(original)]; }
    [[nodiscard]] BlockId id_of(BlockId original) const { return static_cast<BlockId>(first + place_of(original)); }
};

// Copies of `ids` with their pointers among themselves redirected. `ids` is
// always everything a block reaches, so no pointer leaves them.
Clones clone(const Blocks &blocks, const std::vector<BlockId> &ids, std::size_t first) {
    Clones copies;
    copies.first = static_cast<BlockId>(first);
    copies.place.assign(blocks.size(), kNoBlock);
    copies.blocks.reserve(ids.size());
    for (const BlockId id : ids) {
        const Block &block = block_at(blocks, id);
        copies.place[id] = static_cast<BlockId>(copies.blocks.size());
        copies.blocks.push_back({block.data, {}, block.pos});
    }
    for (std::size_t i = 0; i < ids.size(); ++i) {
        for (const Pointer &pointer : blocks[ids[i]].pointers)
            copies.blocks[i].pointers.push_back({pointer.at, copies.id_of(pointer.target)});
    }
    return copies;
}

void append(Blocks &blocks, std::vector<Block> &&more) {
    for (Block &block : more) blocks.push_back(std::move(block));
}

// Puts `bytes` in at `at` (at the end when the block is shorter); pointers
// from `at` on move up.
void insert_bytes(Block &block, std::uint64_t at, Bytes bytes) {
    const std::size_t place = static_cast<std::size_t>(std::min<std::uint64_t>(at, block.data.size()));
    block.data.insert(block.data.begin() + static_cast<std::ptrdiff_t>(place), bytes.begin(), bytes.end());
    for (Pointer &pointer : block.pointers)
        if (pointer.at >= at) pointer.at += bytes.size();
}

// Adds a pointer after the first `count` of a pointer table block.
void insert_pointer(Blocks &blocks, BlockId table_id, std::uint16_t count, BlockId target) {
    Block &table = block_at(blocks, table_id);
    const std::array<std::uint8_t, 4> zero{};
    insert_bytes(table, 4u * std::uint64_t{count}, zero);
    set_pointer(table, 4u * std::uint64_t{count}, target);
}

std::uint32_t first_free_group(const Blocks &blocks) {
    std::array<bool, 128> used{};
    for (const BlockId effect : effects(blocks)) used[load32(block_at(blocks, effect).data, 0u) >> 10u & 0x7Fu] = true;
    for (std::uint32_t group = 0; group < 128u; ++group)
        if (!used[group]) return group;
    fail("the package already holds 128 effects");
}

} // namespace

Package::Package(std::span<const std::uint8_t> bytes) {
    const std::vector<Slot> slots = pointer_slots(bytes);
    // A block starts at 0 and at every pointer's target.
    std::vector<std::uint64_t> starts{0u};
    starts.reserve(slots.size() + 1u);
    for (const Slot &slot : slots) starts.push_back(slot.target);
    std::sort(starts.begin(), starts.end());
    starts.erase(std::unique(starts.begin(), starts.end()), starts.end());
    for (std::size_t i = 0; i < starts.size(); ++i) {
        const std::uint64_t end = i + 1u < starts.size() ? starts[i + 1u] : bytes.size();
        const Bytes part =
            bytes.subspan(static_cast<std::size_t>(starts[i]), static_cast<std::size_t>(end - starts[i]));
        blocks_.push_back({std::vector<std::uint8_t>(part.begin(), part.end()), {}, starts[i]});
    }
    for (const Slot &slot : slots) {
        const std::size_t owner =
            static_cast<std::size_t>(std::upper_bound(starts.begin(), starts.end(), slot.at) - starts.begin()) - 1u;
        const std::size_t target =
            static_cast<std::size_t>(std::lower_bound(starts.begin(), starts.end(), slot.target) - starts.begin());
        set_pointer(blocks_[owner], slot.at - starts[owner], static_cast<BlockId>(target));
    }
}

std::vector<std::uint8_t> Package::to_bytes() const {
    std::vector<std::uint8_t> out;
    std::size_t total = 0u;
    for (const Block &block : blocks_) total += block.data.size() + 15u;
    out.reserve(total);
    std::vector<std::uint64_t> where;
    where.reserve(blocks_.size());
    for (const Block &block : blocks_) {
        const std::uint64_t align = block_align(block.pos);
        out.resize(out.size() + static_cast<std::size_t>((align - out.size() % align) % align));
        where.push_back(out.size());
        out.insert(out.end(), block.data.begin(), block.data.end());
    }
    for (std::size_t i = 0; i < blocks_.size(); ++i) {
        for (const Pointer &pointer : blocks_[i].pointers) {
            if (pointer.target >= where.size()) fail("a pointer names a block the package does not hold");
            store32(out, where[i] + pointer.at, where[pointer.target]);
        }
    }
    store32(out, 0x14u, out.size());
    return out;
}

std::vector<std::string> Package::names() const {
    std::vector<std::string> out;
    for (Name &entry : name_table(blocks_)) out.push_back(std::move(entry.name));
    return out;
}

std::vector<std::uint32_t> Package::ids() const {
    std::vector<std::uint32_t> out;
    for (const Name &entry : name_table(blocks_)) out.push_back(entry.id);
    return out;
}

Copied copy_effect_with_id(Package &target, const Package &source, std::string_view name, std::string_view new_name) {
    // `from` may be `into`: blocks are reached by number, and a deque keeps
    // its blocks where they are when more are added.
    Blocks &into = target.blocks_;
    const Blocks &from = source.blocks_;

    const std::vector<Name> names = name_table(from);
    const auto named = std::find_if(names.begin(), names.end(), [&](const Name &entry) { return entry.name == name; });
    if (named == names.end()) fail(std::string(name) + ": no such effect in the source package");
    const std::uint32_t effect_id = named->id;
    BlockId effect = kNoBlock;
    for (const BlockId id : effects(from)) {
        if (load32(block_at(from, id).data, 0u) == effect_id) {
            effect = id;
            break;
        }
    }
    if (effect == kNoBlock) fail(std::string(name) + ": the source package names it but has no such effect");
    const std::vector<BlockId> source_bodies = bodies(from);
    const std::uint16_t body_number = load16(from[effect].data, 4u);
    if (body_number >= source_bodies.size()) fail(std::string(name) + ": its body is not in the source package");
    const BlockId body = source_bodies[body_number];
    const Used used = resources_of(from, body);

    // Resources: textures first, since models and texture animations name them by id.
    std::array<std::map<std::uint32_t, std::uint32_t>, kKinds> mapping;  // source index -> target index
    std::map<std::uint32_t, std::uint32_t> texture_ids;                  // source texture id -> target's
    for (std::size_t kind = 0; kind < kKinds; ++kind) {
        const std::vector<BlockId> source_refs = refs(from, kind);
        std::vector<BlockId> target_refs = refs(into, kind);
        std::set<std::uint32_t> target_ids;
        for (const BlockId id : target_refs) target_ids.insert(load32(block_at(into, id).data, 4u));
        for (const std::uint32_t index : used[kind]) {  // in rising order
            if (index >= source_refs.size()) fail("a parameter names a resource the source package lacks");
            const Block &ref = block_at(from, source_refs[index]);
            const std::uint32_t ref_id = load32(ref.data, 4u);
            const std::optional<Bytes> original = resource_bytes(from, ref);
            std::optional<std::vector<std::uint8_t>> data;
            if (original) data.emplace(original->begin(), original->end());
            if (data && (kind == kMdl || kind == kAnm)) {
                const IdFields fields = texture_id_fields(kind, *original);
                for (std::uint64_t i = 0; i < fields.count; ++i) {
                    const std::uint64_t at = fields.first + fields.step * i;
                    const std::uint32_t old = load32(*original, at);
                    const auto moved = texture_ids.find(old);
                    store32(*data, at, moved != texture_ids.end() ? moved->second : old);
                }
            }
            // The first of the target's that is the same: a library resource by id, any other by its bytes.
            std::optional<std::uint32_t> match;
            for (std::size_t t = 0; t < target_refs.size() && !match; ++t) {
                const Block &t_ref = block_at(into, target_refs[t]);
                if (!data) {
                    if (load32(t_ref.data, 8u) == 0u && load32(t_ref.data, 4u) == ref_id)
                        match = static_cast<std::uint32_t>(t);
                } else {
                    const std::optional<Bytes> other = resource_bytes(into, t_ref);
                    if (other && std::equal(other->begin(), other->end(), data->begin(), data->end()))
                        match = static_cast<std::uint32_t>(t);
                }
            }
            if (match) {
                mapping[kind][index] = *match;
                if (kind == kTex) texture_ids[ref_id] = load32(block_at(into, target_refs[*match]).data, 4u);
                continue;
            }
            // A new resource. An id the target already uses becomes one past the highest of its family (the
            // id's top byte).
            std::uint64_t new_id = ref_id;
            if (data && target_ids.count(ref_id) != 0u) {
                std::uint32_t highest = ref_id;
                for (const std::uint32_t id : target_ids)
                    if (id >> 24u == ref_id >> 24u) highest = std::max(highest, id);
                new_id = std::uint64_t{highest} + 1u;
            }
            if (new_id > 0xFFFFFFFFu) fail("no resource id is free");
            target_ids.insert(static_cast<std::uint32_t>(new_id));
            if (kind == kTex) texture_ids[ref_id] = static_cast<std::uint32_t>(new_id);
            Block new_ref{ref.data, {}, ref.pos};
            store32(new_ref.data, 4u, new_id);
            if (!data) {
                // A library resource's pointer is to its own reference.
                new_ref.pointers.push_back({12u, static_cast<BlockId>(into.size())});
            } else {
                const Block &old = follow(from, ref, 12u);
                Block payload{old.data, {}, old.pos};
                std::copy(data->begin(), data->end(), payload.data.begin());
                store32(payload.data, kind == kMdl ? 8u : 4u, new_id);
                new_ref.pointers.push_back({12u, static_cast<BlockId>(into.size())});
                into.push_back(std::move(payload));
            }
            const BlockId new_ref_id = static_cast<BlockId>(into.size());
            into.push_back(std::move(new_ref));
            const std::uint16_t held = header_count(into, 0x20u + 2u * kind);
            insert_pointer(into, target_at(into.front(), 0x30u + 4u * kind), held, new_ref_id);
            store16(into.front().data, 0x20u + 2u * kind, held + 1u);
            mapping[kind][index] = held;
            target_refs = refs(into, kind);
        }
    }

    // The body, its resource indices renumbered.
    const std::vector<BlockId> body_blocks = reachable(from, body);
    Clones copies = clone(from, body_blocks, into.size());
    for (const Field &field : parameter_fields(from, body)) {
        const std::int64_t value = load_field(block_at(from, field.table).data, field.at, field.width);
        if (value < 0) continue;
        const auto moved = mapping[field.kind].find(static_cast<std::uint32_t>(value));
        if (moved == mapping[field.kind].end()) fail("a parameter names a resource that was not copied");
        store_field(copies.of(field.table).data, field.at, field.width, moved->second);
    }
    const BlockId new_body = copies.id_of(body);
    append(into, std::move(copies.blocks));
    const std::uint16_t body_index = header_count(into, 0x1Eu);
    insert_pointer(into, target_at(into.front(), 0x2Cu), body_index, new_body);
    store16(into.front().data, 0x1Eu, body_index + 1u);

    // The effect entry and its leaves, under a group of the target's own.
    const std::uint32_t new_id = base(into) | (first_free_group(into) << 10u) | (effect_id & 0x3FFu);
    const std::vector<BlockId> effect_blocks = reachable(from, effect);
    copies = clone(from, effect_blocks, into.size());
    Block &new_effect = copies.of(effect);
    need(new_effect.data.size(), 0u, 6u);
    store32(new_effect.data, 0u, new_id);
    store16(new_effect.data, 4u, body_index);
    const BlockId leaves = target_at(from[effect], 8u);
    const std::uint16_t leaf_count = load16(from[effect].data, 6u);
    for (std::uint64_t j = 0; j < leaf_count; ++j) {
        Block &leaf = copies.of(target_at(block_at(from, leaves), 4u * j));
        const std::uint32_t leaf_id = load32(leaf.data, 0u);
        if ((leaf_id & ~0x3FFu) == (effect_id & ~0x3FFu))
            store32(leaf.data, 0u, (new_id & ~0x3FFu) | (leaf_id & 0x3FFu));
    }
    const BlockId new_effect_id = copies.id_of(effect);
    append(into, std::move(copies.blocks));
    const std::uint16_t held = header_count(into, 0x1Cu);
    insert_pointer(into, target_at(into.front(), 0x28u), held, new_effect_id);
    store16(into.front().data, 0x1Cu, held + 1u);

    // Its name: the text, a zero, and zeros to a multiple of four bytes.
    const std::string_view text = new_name.empty() ? name : new_name;
    Block string;
    for (const char c : text) string.data.push_back(static_cast<std::uint8_t>(c));
    string.data.resize(string.data.size() / 4u * 4u + 4u);
    Block &head = block_at(into, target_at(into.front(), 0x44u));
    Block &entries = block_at(into, target_at(head, 4u));
    const std::uint32_t index = load32(head.data, 0u);
    const std::uint64_t at = 12u * std::uint64_t{index};
    std::vector<std::uint8_t> entry(12u);
    store32(entry, 0u, new_id);
    store32(entry, 4u, text.size());
    insert_bytes(entries, at, entry);
    set_pointer(entries, at + 8u, static_cast<BlockId>(into.size()));
    store32(head.data, 0u, std::uint64_t{index} + 1u);
    into.push_back(std::move(string));
    return {index, new_id};
}

std::size_t copy_effect(Package &target, const Package &source, std::string_view name, std::string_view new_name) {
    return copy_effect_with_id(target, source, name, new_name).index;
}

} // namespace d012::effect_package
