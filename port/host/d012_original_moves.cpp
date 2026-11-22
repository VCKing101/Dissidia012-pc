// Bartz's moves from the first Dissidia, built from the Dissidia 012 disc;
// see d012_original_moves.hpp. Every step here is tools/original_moves.py's,
// in its order: a different order would give other ids and other bytes.
// MIT licence; see port/README.md.

#include "d012_original_moves.hpp"

#include "d012_effect_package.hpp"
#include "d012_gmo_motion.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstring>
#include <deque>
#include <map>
#include <memory>
#include <set>

namespace d012::original_moves {
namespace {

using Span = std::span<const std::uint8_t>;

[[noreturn]] void fail(const std::string &what) { throw Error(what); }

void need(Span data, std::size_t at, std::size_t count) {
    if (at > data.size() || count > data.size() - at) fail("a table ends before its records do");
}
std::uint16_t u16(Span data, std::size_t at) {
    need(data, at, 2u);
    return static_cast<std::uint16_t>(data[at] | data[at + 1u] << 8u);
}
std::int16_t s16(Span data, std::size_t at) { return static_cast<std::int16_t>(u16(data, at)); }
std::uint32_t u32(Span data, std::size_t at) {
    need(data, at, 4u);
    return std::uint32_t(data[at]) | std::uint32_t(data[at + 1u]) << 8u | std::uint32_t(data[at + 2u]) << 16u |
           std::uint32_t(data[at + 3u]) << 24u;
}
void put16(Bytes &data, std::size_t at, std::uint32_t value) {
    need(data, at, 2u);
    data[at] = static_cast<std::uint8_t>(value);
    data[at + 1u] = static_cast<std::uint8_t>(value >> 8u);
}
void put32(Bytes &data, std::size_t at, std::uint32_t value) {
    need(data, at, 4u);
    for (unsigned k = 0u; k < 4u; ++k) data[at + k] = static_cast<std::uint8_t>(value >> (8u * k));
}
void add16(Bytes &out, std::uint32_t value) {
    out.push_back(static_cast<std::uint8_t>(value));
    out.push_back(static_cast<std::uint8_t>(value >> 8u));
}
void add32(Bytes &out, std::uint32_t value) {
    for (unsigned k = 0u; k < 4u; ++k) out.push_back(static_cast<std::uint8_t>(value >> (8u * k)));
}
void add(Bytes &out, Span more) { out.insert(out.end(), more.begin(), more.end()); }
void add(Bytes &out, std::string_view text) { out.insert(out.end(), text.begin(), text.end()); }
// Zeros up to the next multiple of `align`.
void pad(Bytes &out, std::size_t align) { out.resize(out.size() + (align - out.size() % align) % align); }
Bytes slice(Span data, std::size_t at, std::size_t count) {
    need(data, at, count);
    return Bytes(data.begin() + static_cast<std::ptrdiff_t>(at), data.begin() + static_cast<std::ptrdiff_t>(at + count));
}
// As a Python slice: what is there, never past the end.
Bytes lenient(Span data, std::size_t at, std::size_t count) {
    if (at >= data.size()) return {};
    return slice(data, at, std::min(count, data.size() - at));
}
bool starts(Span data, std::string_view magic) {
    return data.size() >= magic.size() && std::memcmp(data.data(), magic.data(), magic.size()) == 0;
}
// The text at `at` up to its zero byte (or the end of `limit` bytes).
std::string text_at(Span data, std::size_t at, std::size_t limit = SIZE_MAX) {
    std::string out;
    for (std::size_t i = at; i < data.size() && i - at < limit && data[i] != 0u; ++i) out.push_back(static_cast<char>(data[i]));
    return out;
}

// ---------------------------------------------------------------------------
// Containers

// `ARC\x01`: u32 count, 8 bytes kept, then {u32 kind, char tag[4], u32 offset,
// u32 size}. A chunk starts on 4 bytes, or on what its kind asks.
struct Arc {
    struct Chunk {
        std::uint32_t kind{};
        std::array<char, 4> tag{};
        Bytes data;
    };
    Bytes extra;
    std::vector<Chunk> chunks;

    explicit Arc(Span data) {
        if (!starts(data, std::string_view("ARC\x01", 4u))) fail("not an ARC archive");
        const std::uint32_t count = u32(data, 4u);
        extra = slice(data, 8u, 8u);
        for (std::uint32_t i = 0u; i < count; ++i) {
            const std::size_t at = 16u + std::size_t(i) * 16u;
            Chunk chunk;
            chunk.kind = u32(data, at);
            need(data, at + 4u, 4u);
            std::memcpy(chunk.tag.data(), data.data() + at + 4u, 4u);
            chunk.data = slice(data, u32(data, at + 8u), u32(data, at + 12u));
            chunks.push_back(std::move(chunk));
        }
    }
    // The first chunk with this tag (when one is given) and kind (when not -1).
    [[nodiscard]] Chunk *find(const char *tag, long kind = -1) {
        for (Chunk &chunk : chunks)
            if ((tag == nullptr || std::memcmp(chunk.tag.data(), tag, 4u) == 0) && (kind < 0 || chunk.kind == std::uint32_t(kind)))
                return &chunk;
        return nullptr;
    }
    [[nodiscard]] Bytes &get(const char *tag, long kind = -1) {
        Chunk *chunk = find(tag, kind);
        if (chunk == nullptr) fail(std::string("no chunk ") + (tag != nullptr ? std::string(tag, 4u) : std::to_string(kind)));
        return chunk->data;
    }
    [[nodiscard]] Bytes to_bytes() const {
        Bytes table, body;
        std::size_t position = 16u + chunks.size() * 16u;
        for (const Chunk &chunk : chunks) {
            const std::size_t align = chunk.kind == 14u ? 64u : (chunk.kind == 3u || chunk.kind == 2u) ? 16u : 4u;
            const std::size_t gap = (align - position % align) % align;
            body.resize(body.size() + gap);
            position += gap;
            add32(table, chunk.kind);
            table.insert(table.end(), chunk.tag.begin(), chunk.tag.end());
            add32(table, static_cast<std::uint32_t>(position));
            add32(table, static_cast<std::uint32_t>(chunk.data.size()));
            add(body, chunk.data);
            position += chunk.data.size();
        }
        Bytes out;
        add(out, std::string_view("ARC\x01", 4u));
        add32(out, static_cast<std::uint32_t>(chunks.size()));
        add(out, extra);
        add(out, table);
        add(out, body);
        return out;
    }
};

// `MPK `: u32 version, u32 count, u32 0, {u32 name offset, u32 data offset,
// u32 size, u32 0}, the names, then the members on 16 bytes.
struct Mpk {
    std::uint32_t version{};
    std::vector<std::pair<std::string, Bytes>> members;

    explicit Mpk(Span data) {
        if (!starts(data, "MPK ")) fail("not an MPK archive");
        version = u32(data, 4u);
        const std::uint32_t count = u32(data, 8u);
        for (std::uint32_t i = 0u; i < count; ++i) {
            const std::size_t at = 16u + std::size_t(i) * 16u;
            const std::uint32_t name_at = u32(data, at);
            need(data, name_at, 1u);
            members.emplace_back(text_at(data, name_at), slice(data, u32(data, at + 4u), u32(data, at + 8u)));
        }
    }
    [[nodiscard]] Bytes &get(std::string_view suffix) {
        for (auto &[name, data] : members)
            if (std::string_view(name).ends_with(suffix)) return data;
        fail("no member *" + std::string(suffix));
    }
    [[nodiscard]] Bytes to_bytes() const {
        Bytes names;
        std::vector<std::size_t> name_at;
        const std::size_t start = 16u + members.size() * 16u;
        for (const auto &[name, data] : members) {
            name_at.push_back(start + names.size());
            add(names, name);
            names.push_back(0u);
        }
        std::size_t position = start + names.size();
        position += (16u - position % 16u) % 16u;
        Bytes table, body;
        for (std::size_t i = 0u; i < members.size(); ++i) {
            const Bytes &data = members[i].second;
            add32(table, static_cast<std::uint32_t>(name_at[i]));
            add32(table, static_cast<std::uint32_t>(position));
            add32(table, static_cast<std::uint32_t>(data.size()));
            add32(table, 0u);
            add(body, data);
            pad(body, 16u);
            position += data.size() + (16u - data.size() % 16u) % 16u;
        }
        Bytes out;
        add(out, "MPK ");
        add32(out, version);
        add32(out, static_cast<std::uint32_t>(members.size()));
        add32(out, 0u);
        add(out, table);
        add(out, names);
        pad(out, 16u);
        add(out, body);
        return out;
    }
};

// ---------------------------------------------------------------------------
// 012's tables

// `count` (at `count_at`) records of `size` bytes from `head`.
std::vector<Bytes> records(Span data, std::size_t size, std::size_t head = 8u, std::size_t count_at = 4u) {
    const std::uint32_t count = u32(data, count_at);
    need(data, head, std::size_t(count) * size);
    std::vector<Bytes> rows;
    for (std::uint32_t i = 0u; i < count; ++i) rows.push_back(slice(data, head + std::size_t(i) * size, size));
    return rows;
}

// `comb`: {u16 6, s16 count, u32 offset[]}, entries {s16 command, s16 stages,
// s16 alt, s16 prerequisite}, then the 8-byte stages.
struct Combo {
    int command{};
    int alt{-1};
    int prerequisite{-1};
    std::vector<Bytes> stages;
};
std::vector<Combo> read_comb(Span data) {
    if (u16(data, 0u) != 6u) fail("comb: not version 6");
    const int count = s16(data, 2u);
    std::vector<Combo> entries;
    for (int i = 0; i < count; ++i) {
        const std::size_t offset = u32(data, 4u + 4u * std::size_t(i));
        Combo entry;
        entry.command = s16(data, offset);
        const int stages = s16(data, offset + 2u);
        entry.alt = s16(data, offset + 4u);
        entry.prerequisite = s16(data, offset + 6u);
        for (int k = 0; k < stages; ++k) entry.stages.push_back(slice(data, offset + 8u + 8u * std::size_t(k), 8u));
        entries.push_back(std::move(entry));
    }
    return entries;
}
Bytes write_comb(const std::vector<Combo> &entries) {
    const std::size_t start = 4u + 4u * entries.size();
    std::vector<std::size_t> offsets;
    Bytes body;
    for (const Combo &entry : entries) {
        offsets.push_back(start + body.size());
        add16(body, static_cast<std::uint32_t>(entry.command));
        add16(body, static_cast<std::uint32_t>(entry.stages.size()));
        add16(body, static_cast<std::uint32_t>(entry.alt));
        add16(body, static_cast<std::uint32_t>(entry.prerequisite));
        for (const Bytes &stage : entry.stages) add(body, stage);
    }
    Bytes out;
    add16(out, 6u);
    add16(out, static_cast<std::uint32_t>(entries.size()));
    for (const std::size_t offset : offsets) add32(out, static_cast<std::uint32_t>(offset));
    add(out, body);
    return out;
}

// `magi` projectiles: "MAGI", u32 version, u32 count, u32 floats, 0xCC-byte
// rows {0x40 bytes, 138 u8 indices into the float pool, 2 bytes}, the pool.
constexpr std::size_t kMagiRow = 0xCCu;
constexpr std::size_t kMagiValues = 3u * 46u;
struct Magi {
    Bytes head;                         // 0x40 bytes
    std::vector<std::uint32_t> values;  // a new row's 138 floats, as their bits
    std::optional<Bytes> indices;       // a row of the disc keeps its own
    Bytes tail;
};
struct MagiTable {
    std::uint32_t version{};
    std::vector<Magi> rows;
    std::vector<std::uint32_t> pool;  // the floats' bits
};
MagiTable read_magi(Span data) {
    if (!starts(data, "MAGI")) fail("magi: no MAGI magic");
    MagiTable table;
    table.version = u32(data, 4u);
    const std::uint32_t count = u32(data, 8u), floats = u32(data, 12u);
    for (std::uint32_t i = 0u; i < floats; ++i) {
        const std::uint32_t bits = u32(data, 16u + std::size_t(count) * kMagiRow + 4u * std::size_t(i));
        // Python reads the pool as floats and writes them back; a NaN would
        // not come back bit for bit, and no fighter's pool has one.
        if ((bits & 0x7F800000u) == 0x7F800000u && (bits & 0x007FFFFFu) != 0u) fail("magi: a pool value is not a number");
        table.pool.push_back(bits);
    }
    for (std::uint32_t i = 0u; i < count; ++i) {
        const std::size_t at = 16u + std::size_t(i) * kMagiRow;
        Magi row;
        row.head = slice(data, at, 0x40u);
        row.indices = slice(data, at + 0x40u, kMagiValues);
        row.tail = slice(data, at + 0x40u + kMagiValues, kMagiRow - 0x40u - kMagiValues);
        table.rows.push_back(std::move(row));
    }
    return table;
}
// Rows of the disc keep their pool indices; a new row's values are found in
// the pool or put at its end (128 values at most).
Bytes write_magi(const MagiTable &table) {
    std::vector<std::uint32_t> pool = table.pool;
    std::vector<const Magi *> rows;
    for (const Magi &row : table.rows) rows.push_back(&row);
    std::stable_sort(rows.begin(), rows.end(), [](const Magi *a, const Magi *b) { return u16(a->head, 0u) < u16(b->head, 0u); });
    Bytes body;
    for (const Magi *row : rows) {
        Bytes indices;
        if (row->indices) {
            indices = *row->indices;
        } else {
            for (const std::uint32_t bits : row->values) {
                auto found = std::find(pool.begin(), pool.end(), bits);
                if (found == pool.end()) {
                    pool.push_back(bits);
                    found = pool.end() - 1;
                }
                indices.push_back(static_cast<std::uint8_t>(found - pool.begin()));
            }
        }
        add(body, row->head);
        add(body, indices);
        if (row->tail.empty()) body.resize(body.size() + 2u);
        else add(body, row->tail);
    }
    if (pool.size() > 128u) fail("magi: " + std::to_string(pool.size()) + " pool values, the most is 128");
    Bytes out;
    add(out, "MAGI");
    add32(out, table.version);
    add32(out, static_cast<std::uint32_t>(table.rows.size()));
    add32(out, static_cast<std::uint32_t>(pool.size()));
    add(out, body);
    for (const std::uint32_t bits : pool) add32(out, bits);
    return out;
}

// Event tracks (kind 8): u32 version, u32 count, u32 offset[]; a track is
// {u16 animation, u8 ranged, u8 instant, u16 where the instant events start,
// u16 size}, ranged events {12-byte header, u32 args[header[5]]}, then
// instant events {8-byte header, u32 args[header[3]]}.
struct Event {
    Bytes head;
    std::vector<std::uint32_t> args;
};
struct Track {
    std::uint32_t anim{};
    std::vector<Event> ranged, instant;
};
struct TrackTable {
    std::uint32_t version{};
    std::vector<Track> tracks;
};
TrackTable read_tracks(Span data) {
    TrackTable table;
    table.version = u32(data, 0u);
    const std::uint32_t count = u32(data, 4u);
    for (std::uint32_t i = 0u; i < count; ++i) {
        const std::size_t offset = u32(data, 8u + 4u * std::size_t(i));
        Track track;
        track.anim = u16(data, offset);
        need(data, offset, 8u);
        const unsigned ranged = data[offset + 2u], instant = data[offset + 3u];
        const std::size_t instant_at = u16(data, offset + 4u), size = u16(data, offset + 6u);
        std::size_t cursor = offset + 8u;
        const auto read_events = [&](unsigned events, std::size_t head_size, std::size_t argc_at, std::vector<Event> &out) {
            for (unsigned k = 0u; k < events; ++k) {
                Event event;
                event.head = slice(data, cursor, head_size);
                const std::size_t argc = event.head[argc_at];
                for (std::size_t a = 0u; a < argc; ++a) event.args.push_back(u32(data, cursor + head_size + 4u * a));
                cursor += head_size + 4u * argc;
                out.push_back(std::move(event));
            }
        };
        read_events(ranged, 12u, 5u, track.ranged);
        if (cursor != offset + instant_at) fail("a track's ranged events do not end where its instant events start");
        read_events(instant, 8u, 3u, track.instant);
        if (cursor != offset + size) fail("a track's events do not end where the track does");
        table.tracks.push_back(std::move(track));
    }
    return table;
}
Bytes write_tracks(const TrackTable &table) {
    std::vector<Bytes> bodies;
    for (const Track &track : table.tracks) {
        Bytes events;
        const auto write_events = [&](const std::vector<Event> &list, std::size_t argc_at) {
            for (const Event &event : list) {
                Bytes head = event.head;
                head[argc_at] = static_cast<std::uint8_t>(event.args.size());
                add(events, head);
                for (const std::uint32_t arg : event.args) add32(events, arg);
            }
        };
        write_events(track.ranged, 5u);
        const std::size_t instant_at = 8u + events.size();
        write_events(track.instant, 3u);
        Bytes body;
        add16(body, track.anim);
        body.push_back(static_cast<std::uint8_t>(track.ranged.size()));
        body.push_back(static_cast<std::uint8_t>(track.instant.size()));
        add16(body, static_cast<std::uint32_t>(instant_at));
        add16(body, static_cast<std::uint32_t>(8u + events.size()));
        add(body, events);
        bodies.push_back(std::move(body));
    }
    Bytes out;
    add32(out, table.version);
    add32(out, static_cast<std::uint32_t>(table.tracks.size()));
    std::size_t position = 8u + 4u * table.tracks.size();
    for (const Bytes &body : bodies) {
        add32(out, static_cast<std::uint32_t>(position));
        position += body.size();
    }
    for (const Bytes &body : bodies) add(out, body);
    return out;
}
unsigned event_type(const Event &event, bool ranged) { return event.head[ranged ? 4u : 2u]; }

// CPU weights (`brat`): 16-byte head, u32 rows, rows {u32 command, 6 x {u16
// offset, u16 count}}, u32 pool size, the pool.
struct Brat {
    Bytes head;
    struct Row {
        std::uint32_t command{};
        std::array<std::pair<std::uint32_t, std::uint32_t>, 6> situations{};
    };
    std::vector<Row> rows;
    Bytes pool;
};
Brat read_brat(Span data) {
    Brat brat;
    brat.head = slice(data, 0u, 16u);
    const std::uint32_t count = u32(data, 16u);
    for (std::uint32_t i = 0u; i < count; ++i) {
        Brat::Row row;
        row.command = u32(data, 20u + 28u * std::size_t(i));
        for (std::size_t k = 0u; k < 6u; ++k)
            row.situations[k] = {u16(data, 24u + 28u * std::size_t(i) + 4u * k), u16(data, 26u + 28u * std::size_t(i) + 4u * k)};
        brat.rows.push_back(row);
    }
    const std::size_t pool_at = 20u + 28u * std::size_t(count);
    brat.pool = lenient(data, pool_at + 4u, u32(data, pool_at));
    return brat;
}
Bytes write_brat(const Brat &brat) {
    Bytes out = brat.head;
    add32(out, static_cast<std::uint32_t>(brat.rows.size()));
    for (const Brat::Row &row : brat.rows) {
        add32(out, row.command);
        for (const auto &[offset, count] : row.situations) {
            if (offset > 0xFFFFu || count > 0xFFFFu) fail("brat: the pool is too long for its offsets");
            add16(out, offset);
            add16(out, count);
        }
    }
    add32(out, static_cast<std::uint32_t>(brat.pool.size()));
    add(out, brat.pool);
    pad(out, 4u);
    return out;
}

// A table of records behind "MAGI"-like heads: FEPA (effect placement, 260
// bytes) and FXSE (effect sounds, 320 bytes) are {magic, u32 version, u32
// size, u32 count, records}.
Bytes write_headed(std::string_view magic, std::uint32_t version, const std::vector<Bytes> &rows) {
    Bytes body;
    for (const Bytes &row : rows) add(body, row);
    Bytes out;
    add(out, magic);
    add32(out, version);
    add32(out, static_cast<std::uint32_t>(16u + body.size()));
    add32(out, static_cast<std::uint32_t>(rows.size()));
    add(out, body);
    return out;
}

// A fighter's sound effect bank (`SSCF`): u32 slots at +12, 32-byte entries
// {u32 kind, u32 offset, u32 size, ...} from +0x20. A clip is put into an
// empty slot, its data at the end, so nothing else moves.
struct SoundBank {
    static constexpr std::uint32_t kEmpty = 0x00200000u;
    Bytes data;

    SoundBank() = default;
    explicit SoundBank(Span bytes) : data(bytes.begin(), bytes.end()) {
        if (!starts(bytes, "SSCF")) fail("not an SSCF sound bank");
    }
    [[nodiscard]] std::uint32_t number() const {
        const std::uint32_t low = u16(data, 0x12u);
        return low != 0u ? low : u32(data, 0x14u);
    }
    [[nodiscard]] std::uint32_t slots() const { return u32(data, 12u); }
    [[nodiscard]] std::array<std::uint32_t, 8> entry(std::uint64_t slot) const {
        std::array<std::uint32_t, 8> words{};
        if (slot > 0x00FFFFFFu) fail("a sound slot is out of range");
        for (std::size_t k = 0u; k < 8u; ++k) words[k] = u32(data, 0x20u + 32u * std::size_t(slot) + 4u * k);
        return words;
    }
    // The slot's clip; nothing for a slot that holds none.
    [[nodiscard]] std::optional<Bytes> clip(std::uint64_t slot) const {
        const auto words = entry(slot);
        if (words[0] != 0u && words[0] != 0x00010000u) return std::nullopt;
        return lenient(data, words[1], words[2]);
    }
    [[nodiscard]] std::optional<std::uint32_t> find(const Bytes &wanted) const {
        for (std::uint32_t slot = 0u; slot < slots(); ++slot)
            if (const auto have = clip(slot); have && *have == wanted) return slot;
        return std::nullopt;
    }
    std::uint32_t add_clip(std::array<std::uint32_t, 8> words, const Bytes &wanted) {
        std::uint32_t slot = 0u;
        for (;; ++slot) {
            if (slot >= slots()) fail("the sound bank has no empty slot left");
            if (entry(slot)[0] == kEmpty) break;
        }
        pad(data, 16u);
        words[1] = static_cast<std::uint32_t>(data.size());
        words[2] = static_cast<std::uint32_t>(wanted.size());
        add(data, wanted);
        for (std::size_t k = 0u; k < 8u; ++k) put32(data, 0x20u + 32u * std::size_t(slot) + 4u * k, words[k]);
        put32(data, 8u, static_cast<std::uint32_t>(data.size()));
        return slot;
    }
};
// The slot of `bank` that holds `source`'s clip `slot`: an equal clip, or a
// copy in an empty slot.
std::optional<std::uint32_t> bring_sound(SoundBank &bank, const SoundBank &source, std::uint64_t slot) {
    const auto clip = source.clip(slot);
    if (!clip) return std::nullopt;
    if (const auto found = bank.find(*clip)) return found;
    return bank.add_clip(source.entry(slot), *clip);
}

// The game's key for hiding message text (main 0x089D68AA).
constexpr std::array<std::uint16_t, 16> kTextKey = {0xED98, 0x90AB, 0x5E28, 0x8442, 0x32B1, 0x2DDA, 0xA657, 0xC8BF,
                                                    0x0033, 0x579A, 0xF227, 0x4B4C, 0x36A7, 0x927D, 0x739D, 0x84CE};

// A moveset's banner texts: `mlng`, u32 tables, u32 offset[], u32 size[],
// the tables. A table is u16 count, u16 offset[] (in words; 0: none), then
// strings {u16 bytes, the text hidden by its index}.
using Strings = std::vector<std::optional<Bytes>>;  // each plain, as bytes
Bytes mask(Span text, std::size_t index) {
    Bytes out;
    for (std::size_t k = 0u; k < text.size() / 2u; ++k)
        add16(out, static_cast<std::uint32_t>(u16(text, 2u * k) ^ kTextKey[(index + k) & 15u]));
    return out;
}
std::vector<Strings> read_banners(Span chunk) {
    if (!starts(chunk, "mlng")) fail("text: not an mlng container");
    const std::uint32_t count = u32(chunk, 4u);
    std::vector<Strings> tables;
    for (std::uint32_t t = 0u; t < count; ++t) {
        const Bytes table = lenient(chunk, u32(chunk, 8u + 4u * std::size_t(t)), u32(chunk, 8u + 4u * (std::size_t(count) + t)));
        Strings strings;
        const std::size_t entries = u16(table, 0u);
        for (std::size_t i = 0u; i < entries; ++i) {
            const std::size_t at = std::size_t(u16(table, 2u + 2u * i)) * 2u;
            if (at == 0u) {
                strings.emplace_back();
                continue;
            }
            strings.emplace_back(mask(lenient(table, at + 2u, u16(table, at)), i));
        }
        tables.push_back(std::move(strings));
    }
    return tables;
}
Bytes write_banners(const std::vector<Strings> &tables) {
    std::vector<Bytes> built;
    for (const Strings &strings : tables) {
        const std::size_t start = 2u + 2u * strings.size();
        std::vector<std::size_t> offsets;
        Bytes body;
        for (std::size_t i = 0u; i < strings.size(); ++i) {
            if (!strings[i]) {
                offsets.push_back(0u);
                continue;
            }
            offsets.push_back((start + body.size()) / 2u);
            add16(body, static_cast<std::uint32_t>(strings[i]->size()));
            add(body, mask(*strings[i], i));
        }
        Bytes table;
        add16(table, static_cast<std::uint32_t>(strings.size()));
        for (const std::size_t offset : offsets) add16(table, static_cast<std::uint32_t>(offset));
        add(table, body);
        built.push_back(std::move(table));
    }
    Bytes out;
    add(out, "mlng");
    add32(out, static_cast<std::uint32_t>(built.size()));
    std::size_t position = 8u + 8u * built.size();
    for (const Bytes &table : built) {
        add32(out, static_cast<std::uint32_t>(position));
        position += table.size();
    }
    for (const Bytes &table : built) add32(out, static_cast<std::uint32_t>(table.size()));
    for (const Bytes &table : built) add(out, table);
    return out;
}

// ---------------------------------------------------------------------------
// A fighter's moveset, decoded: a 012 `.objx` archive with its tables.

struct Fighter {
    Arc arc;
    std::uint32_t attacks_version{}, hits_version{}, motions_version{};
    std::vector<Bytes> attacks;  // plya, 0x74 bytes
    std::vector<Bytes> hits;     // atkp, 0x44 bytes
    std::vector<Combo> combos;
    MagiTable projectiles;
    std::vector<Bytes> motions;  // kind 7: {u16 motion id, u16 animation, u16 variant, u16}
    TrackTable tracks;
    std::uint64_t shape_mask{};
    std::vector<Bytes> shapes;   // kind 5: 0x20 bytes, byte 0 the hitbox group's id
    Brat brat;
    Mpk effe;
    std::uint32_t fepa_version{}, fxse_version{};
    std::vector<Bytes> fepa, fxse;
    effect_package::Package fep;
    std::vector<Strings> banners;
    SoundBank bank;

    explicit Fighter(Span data)
        : arc(data), effe(arc.get("effe")), fep(Span(effe.get(".fep"))) {
        const Bytes &plya = arc.get("plya");
        attacks_version = u32(plya, 0u);
        attacks = records(plya, 0x74u);
        const Bytes &atkp = arc.get("atkp");
        hits_version = u32(atkp, 0u);
        hits = records(atkp, 0x44u);
        combos = read_comb(arc.get("comb"));
        projectiles = read_magi(arc.get("magi"));
        const Bytes &motion_table = arc.get(nullptr, 7);
        motions_version = u32(motion_table, 0u);
        motions = records(motion_table, 8u);
        tracks = read_tracks(arc.get(nullptr, 8));
        const Bytes &shape_table = arc.get(nullptr, 5);
        shape_mask = u32(shape_table, 0u) | std::uint64_t(u32(shape_table, 4u)) << 32u;
        const std::uint32_t shape_count = u32(shape_table, 8u);
        for (std::uint32_t i = 0u; i < shape_count; ++i) shapes.push_back(slice(shape_table, 16u + 32u * std::size_t(i), 32u));
        brat = read_brat(arc.get("brat"));
        const Bytes &feb = effe.get(".feb");
        if (!starts(feb, "FEPA")) fail("no FEPA magic");
        fepa_version = u32(feb, 4u);
        fepa = records(feb, 260u, 16u, 12u);
        const Bytes &fxseb = effe.get(".fxseb");
        if (!starts(fxseb, "FXSE")) fail("no FXSE magic");
        fxse_version = u32(fxseb, 4u);
        fxse = records(fxseb, 320u, 16u, 12u);
        banners = read_banners(arc.get("text"));
        bank = SoundBank(arc.get(nullptr, 14));
    }

    [[nodiscard]] Bytes to_bytes() {
        const auto table = [](std::uint32_t version, const std::vector<Bytes> &rows) {
            Bytes out;
            add32(out, version);
            add32(out, static_cast<std::uint32_t>(rows.size()));
            for (const Bytes &row : rows) add(out, row);
            return out;
        };
        arc.get("plya") = table(attacks_version, attacks);
        std::stable_sort(hits.begin(), hits.end(), [](const Bytes &a, const Bytes &b) { return u32(a, 0u) < u32(b, 0u); });
        arc.get("atkp") = table(hits_version, hits);
        arc.get("comb") = write_comb(combos);
        arc.get("magi") = write_magi(projectiles);
        std::vector<Bytes> sorted = motions;
        std::stable_sort(sorted.begin(), sorted.end(), [](const Bytes &a, const Bytes &b) { return u16(a, 0u) < u16(b, 0u); });
        arc.get(nullptr, 7) = table(motions_version, sorted);
        arc.get(nullptr, 8) = write_tracks(tracks);
        Bytes shape_table;
        add32(shape_table, static_cast<std::uint32_t>(shape_mask));
        add32(shape_table, static_cast<std::uint32_t>(shape_mask >> 32u));
        add32(shape_table, static_cast<std::uint32_t>(shapes.size()));
        add32(shape_table, 0u);
        for (const Bytes &row : shapes) add(shape_table, row);
        arc.get(nullptr, 5) = std::move(shape_table);
        arc.get("brat") = write_brat(brat);
        effe.get(".feb") = write_headed("FEPA", fepa_version, fepa);
        effe.get(".fep") = fep.to_bytes();
        effe.get(".fxseb") = write_headed("FXSE", fxse_version, fxse);
        arc.get("effe") = effe.to_bytes();
        arc.get("text") = write_banners(banners);
        arc.get(nullptr, 14) = bank.data;
        return arc.to_bytes();
    }
};

// An effect's name without its package's prefix: P0041_SOUL_EATER_BODY_LEAF_0
// -> SOUL_EATER_BODY_LEAF_0.
std::string effect_stem(std::string_view name) {
    const auto digit = [](char c) { return c >= '0' && c <= '9'; };
    if (name.size() >= 6u && name[0] == 'P' && digit(name[1]) && digit(name[2]) && digit(name[3]) && digit(name[4]) &&
        name[5] == '_')
        return std::string(name.substr(6u));
    return std::string(name);
}

// Another 012 fighter's effects, effect sounds and sound bank: where an effect
// or a clip the moves need is copied from.
struct Source {
    effect_package::Package fep;
    std::vector<Bytes> fxse;
    SoundBank bank;
    std::map<std::string, std::pair<std::size_t, std::string>> by_stem;  // an effect's index and name

    explicit Source(Arc arc) : Source(arc, Mpk(arc.get("effe"))) {}

private:
    Source(Arc &arc, Mpk effe) : fep(Span(effe.get(".fep"))), bank(arc.get(nullptr, 14)) {
        const Bytes &fxseb = effe.get(".fxseb");
        if (!starts(fxseb, "FXSE")) fail("no FXSE magic");
        fxse = records(fxseb, 320u, 16u, 12u);
        const std::vector<std::string> names = fep.names();
        for (std::size_t i = 0u; i < names.size(); ++i) by_stem[effect_stem(names[i])] = {i, names[i]};
    }
};

// ---------------------------------------------------------------------------
// Models (Sony GMO): just enough to add motions. A chunk is {u16 type, u16
// header size, u32 size}; one with children (type bit 0x8000 clear) goes on
// {u32 where its children start, u32 data}, its name, then the children.

constexpr std::string_view kGmoMagic("OMG.00.1PSP\0\0\0\0\0", 16u);
constexpr unsigned kGmoModel = 0x0003u, kGmoMotion = 0x000Bu;

struct GmoChunk {
    std::size_t at{}, size{};
    unsigned type{};
};
// The children of the chunk at `at`.
std::vector<GmoChunk> gmo_children(Span data, std::size_t at) {
    std::vector<GmoChunk> kids;
    if ((u16(data, at) & 0x8000u) != 0u) return kids;
    const std::size_t end = at + u32(data, at + 4u);
    for (std::size_t cursor = at + u32(data, at + 8u); cursor < end;) {
        const std::size_t size = u32(data, cursor + 4u);
        if (size < 8u) fail("a model chunk has no size");
        need(data, cursor, size);
        kids.push_back({cursor, size, u16(data, cursor)});
        cursor += size;
    }
    return kids;
}
std::string gmo_name(Span data, const GmoChunk &chunk) {
    const std::size_t header = u16(data, chunk.at + 2u);
    return header > 16u ? text_at(data, chunk.at + 16u, header - 16u) : std::string();
}
struct GmoModel {
    std::size_t at{};               // the model chunk
    std::vector<GmoChunk> motions;  // in order
};
GmoModel gmo_model(Span data) {
    if (!starts(data, kGmoMagic)) fail("not a GMO model");
    for (const GmoChunk &chunk : gmo_children(data, 16u)) {
        if (chunk.type != kGmoModel) continue;
        GmoModel model;
        model.at = chunk.at;
        for (const GmoChunk &kid : gmo_children(data, chunk.at))
            if (kid.type == kGmoMotion) model.motions.push_back(kid);
        if (!model.motions.empty()) return model;
    }
    fail("the model has no motions");
}
// A copy of a motion chunk under another name: the header grows or shrinks
// with the name.
Bytes renamed_motion(Span raw, std::string_view name) {
    const std::size_t header = u16(raw, 2u);
    Bytes name_bytes(name.begin(), name.end());
    name_bytes.push_back(0u);
    pad(name_bytes, 4u);
    const std::uint32_t shift = static_cast<std::uint32_t>(16u + name_bytes.size()) - static_cast<std::uint32_t>(header);
    Bytes out = slice(raw, 0u, 16u);
    put16(out, 2u, static_cast<std::uint32_t>(header) + shift);
    put32(out, 4u, u32(raw, 4u) + shift);
    put32(out, 8u, u32(raw, 8u) + shift);
    put32(out, 12u, u32(raw, 12u) + shift);
    add(out, name_bytes);
    need(raw, header, 0u);
    add(out, raw.subspan(header));
    return out;
}
// The model with `chunks` added after its last motion.
Bytes gmo_append_motions(Span data, const std::vector<Bytes> &chunks) {
    const GmoModel model = gmo_model(data);
    const std::size_t root_size = u32(data, 16u + 4u);
    need(data, 16u, root_size);
    const std::size_t insert = model.motions.back().at + model.motions.back().size;
    Bytes out = slice(data, 0u, insert);
    std::size_t added = 0u;
    for (const Bytes &chunk : chunks) {
        add(out, chunk);
        added += chunk.size();
    }
    add(out, data.subspan(insert, 16u + root_size - insert));
    put32(out, 16u + 4u, static_cast<std::uint32_t>(root_size + added));
    put32(out, model.at + 4u, static_cast<std::uint32_t>(u32(data, model.at + 4u) + added));
    return out;
}

// ---------------------------------------------------------------------------
// The recipe: the original moves' records, in 012's layouts, and where each
// animation, effect and sound comes from on the 012 disc.

const json::Value &member(const json::Value &object, std::string_view name) {
    const json::Value *value = object.find(name);
    if (value == nullptr) fail("the recipe has no \"" + std::string(name) + "\"");
    return *value;
}
long long whole(const json::Value &value) {
    if (!value.is(json::Value::Kind::Number)) fail("the recipe has something else where a number goes");
    return static_cast<long long>(value.number);
}
long long whole(std::string_view text) {
    long long value = 0;
    bool negative = false;
    for (std::size_t i = 0u; i < text.size(); ++i) {
        if (i == 0u && text[i] == '-') negative = true;
        else if (text[i] >= '0' && text[i] <= '9') value = value * 10 + (text[i] - '0');
        else fail("the recipe has a key that is not a number");
    }
    return negative ? -value : value;
}
Bytes hex(const json::Value &value) {
    if (!value.is(json::Value::Kind::String) || value.text.size() % 2u != 0u) fail("the recipe has a record that is not hex");
    const auto digit = [](char c) -> unsigned {
        if (c >= '0' && c <= '9') return unsigned(c - '0');
        if (c >= 'a' && c <= 'f') return unsigned(c - 'a') + 10u;
        if (c >= 'A' && c <= 'F') return unsigned(c - 'A') + 10u;
        fail("the recipe has a record that is not hex");
    };
    Bytes out;
    for (std::size_t i = 0u; i < value.text.size(); i += 2u)
        out.push_back(static_cast<std::uint8_t>(digit(value.text[i]) << 4u | digit(value.text[i + 1u])));
    return out;
}
std::vector<Bytes> hex_list(const json::Value &value) {
    std::vector<Bytes> out;
    for (const json::Value &item : value.items) out.push_back(hex(item));
    return out;
}

struct Animation {
    long long index{};
    std::string name;
    bool rebuilt{};  // from another fighter's animation; else 012 Bartz's own of this name
    std::string owner;
    gmo_motion::Retarget retarget;
};
using Units = std::vector<std::uint16_t>;  // a text, in UTF-16 units
struct AbilitySource {
    long long command{};
    Bytes row;  // 40 bytes, 012's ablt layout
    std::vector<std::pair<std::string, Units>> names, helps;  // by language
    std::optional<int> level;                                 // none: his from the start
    std::optional<std::vector<long long>> mastery;            // the commands whose mastery gives it
};
struct Recipe {
    std::vector<Bytes> attacks, hits, motions, shapes, fepa;
    std::vector<Combo> combos;
    std::vector<Magi> projectiles;
    std::vector<Track> tracks;
    std::map<std::uint32_t, std::string> effect_names;
    std::map<std::string, std::pair<std::string, std::string>> effect_from;  // name -> (fighter, its effect)
    std::map<long long, std::optional<std::pair<std::string, long long>>> sounds;  // slot -> same, or (fighter, slot)
    std::vector<Strings> banners;
    std::vector<Animation> animations;
    std::vector<AbilitySource> abilities;
    std::vector<std::pair<long long, long long>> commands;  // original command -> new command, in order

    explicit Recipe(const json::Value &data) {
        attacks = hex_list(member(data, "attacks"));
        hits = hex_list(member(data, "hits"));
        motions = hex_list(member(data, "motions"));
        shapes = hex_list(member(data, "shapes"));
        fepa = hex_list(member(data, "effect_records"));
        for (const auto &[key, stages] : member(data, "commands").members) {
            Combo combo;
            combo.command = static_cast<int>(whole(key));
            combo.stages = hex_list(stages);
            combos.push_back(std::move(combo));
        }
        for (const json::Value &row : member(data, "projectiles").items) {
            Magi magi;
            magi.head = hex(member(row, "head"));
            for (const json::Value &triple : member(row, "params").items)
                for (const json::Value &value : triple.items)
                    magi.values.push_back(std::bit_cast<std::uint32_t>(static_cast<float>(value.number)));
            if (magi.head.size() != 0x40u || magi.values.size() != kMagiValues) fail("the recipe has a projectile of another size");
            projectiles.push_back(std::move(magi));
        }
        for (const json::Value &source : member(data, "tracks").items) {
            Track track;
            track.anim = static_cast<std::uint32_t>(whole(member(source, "anim")));
            const auto events = [](const json::Value &list, std::vector<Event> &out) {
                for (const json::Value &item : list.items) {
                    if (item.items.size() != 2u) fail("the recipe has an event that is not a head and arguments");
                    Event event;
                    event.head = hex(item.items[0]);
                    for (const json::Value &arg : item.items[1].items) event.args.push_back(static_cast<std::uint32_t>(whole(arg)));
                    out.push_back(std::move(event));
                }
            };
            events(member(source, "ranged"), track.ranged);
            events(member(source, "instant"), track.instant);
            tracks.push_back(std::move(track));
        }
        for (const auto &[key, name] : member(data, "effect_names").members)
            effect_names[static_cast<std::uint32_t>(whole(key))] = name.text;
        for (const auto &[name, from] : member(data, "effect_from").members) {
            if (from.items.size() != 2u) fail("the recipe names an effect's source wrongly");
            effect_from[name] = {from.items[0].text, from.items[1].text};
        }
        for (const auto &[key, plan] : member(data, "sounds").members) {
            if (plan.is(json::Value::Kind::String)) sounds[whole(key)] = std::nullopt;
            else if (plan.items.size() == 2u) sounds[whole(key)] = std::pair{plan.items[0].text, whole(plan.items[1])};
            else fail("the recipe names a sound's source wrongly");
        }
        for (const json::Value &table : member(data, "banners").items) {
            Strings strings;
            for (const json::Value &text : table.items) {
                if (text.is(json::Value::Kind::Null)) strings.emplace_back();
                else strings.emplace_back(hex(text));
            }
            banners.push_back(std::move(strings));
        }
        for (const auto &[key, info] : member(data, "animations").members) {
            Animation animation;
            animation.index = whole(key);
            animation.name = member(info, "name").text;
            if (const json::Value *owner = info.find("owner")) {
                animation.rebuilt = true;
                animation.owner = owner->text;
                gmo_motion::Retarget &r = animation.retarget;
                r.motion = member(info, "motion").text;
                r.frames = static_cast<int>(whole(member(info, "frames")));
                r.mirror = member(info, "mirror").boolean;
                r.speed = member(info, "speed").number;
                r.start = member(info, "start").number;
                r.root_scale = member(info, "root_scale").number;
                r.trans_scale = member(info, "travel_scale").number;
                if (const json::Value *follow = info.find("follow"))
                    for (const auto &[bone, other] : follow->members) r.follow.emplace_back(bone, other.text);
                if (const json::Value *held = info.find("held"))
                    for (const auto &[bone, pose] : held->members) {
                        gmo_motion::Held entry;
                        if (const json::Value *rotate = pose.find("rotate")) {
                            if (rotate->items.size() != 4u) fail("the recipe has a held turn that is not four numbers");
                            entry.rotate.emplace();
                            for (std::size_t k = 0u; k < 4u; ++k) (*entry.rotate)[k] = rotate->items[k].number;
                        }
                        if (const json::Value *translate = pose.find("translate")) {
                            if (translate->items.size() != 3u) fail("the recipe has a held place that is not three numbers");
                            entry.translate.emplace();
                            for (std::size_t k = 0u; k < 3u; ++k) (*entry.translate)[k] = translate->items[k].number;
                        }
                        r.held.emplace_back(bone, entry);
                    }
                if (const json::Value *scales = info.find("scales"))
                    for (const auto &[bone, keys] : scales->members) {
                        std::vector<gmo_motion::ScaleKey> list;
                        for (const json::Value &key_value : keys.items) {
                            if (key_value.items.size() != 2u || key_value.items[1].items.size() != 3u)
                                fail("the recipe has a scale key that is not a frame and three numbers");
                            gmo_motion::ScaleKey scale;
                            scale.frame = key_value.items[0].number;
                            for (std::size_t k = 0u; k < 3u; ++k) scale.value[k] = key_value.items[1].items[k].number;
                            list.push_back(scale);
                        }
                        r.scales.emplace_back(bone, std::move(list));
                    }
            }
            animations.push_back(std::move(animation));
        }
        const auto texts = [](const json::Value &by_language) {
            std::vector<std::pair<std::string, Units>> out;
            for (const auto &[language, units] : by_language.members) {
                Units text;
                for (const json::Value &unit : units.items) text.push_back(static_cast<std::uint16_t>(whole(unit)));
                out.emplace_back(language, std::move(text));
            }
            return out;
        };
        for (const auto &[key, info] : member(data, "abilities").members) {
            AbilitySource ability;
            ability.command = whole(key);
            ability.row = hex(member(info, "row"));
            if (ability.row.size() != 40u) fail("the recipe has an ability record of another size");
            ability.names = texts(member(info, "names"));
            ability.helps = texts(member(info, "helps"));
            if (const json::Value &level = member(info, "level"); !level.is(json::Value::Kind::Null))
                ability.level = static_cast<int>(whole(level));
            if (const json::Value &mastery = member(info, "mastery"); !mastery.is(json::Value::Kind::Null)) {
                ability.mastery.emplace();
                for (const json::Value &command : mastery.items)
                    ability.mastery->push_back(command.is(json::Value::Kind::Null) ? -1 : whole(command));
            }
            abilities.push_back(std::move(ability));
        }
        for (const auto &[key, command] : member(data, "command_ids").members) commands.emplace_back(whole(key), whole(command));
    }

    [[nodiscard]] const Animation &animation(long long index) const {
        for (const Animation &animation : animations)
            if (animation.index == index) return animation;
        fail("the recipe has no animation " + std::to_string(index));
    }
};

// ---------------------------------------------------------------------------
// The disc, as the build reads it.

constexpr std::string_view kCode = "fiv100";       // Bartz
constexpr std::string_view kCodeUpper = "FIV100";
constexpr std::uint32_t kCharId = 9u;
constexpr std::array<std::string_view, 4> kModels = {"1P", "2P", "3P", "ZAKO"};
constexpr std::array<std::string_view, 6> kLanguages = {"EN", "FR", "DE", "IT", "ES", "JP"};
constexpr std::string_view kMainBin = "general_archive/main/main.bin";
std::string names_archive(std::string_view language) { return "general_archive/main/" + std::string(language) + "/main_lang.bin"; }
std::string help_pack(std::string_view language) { return "text/" + std::string(language) + "/mess_pk_customize_ability_0.bin"; }
std::string upper(std::string_view text) {
    std::string out(text);
    for (char &c : out)
        if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
    return out;
}

struct Disc {
    const Reader &read;
    Bytes model;  // 012 Bartz's 1P model
    std::map<std::string, std::unique_ptr<Source>, std::less<>> sources;
    std::map<std::string, std::unique_ptr<Bytes>, std::less<>> skeleton_files;  // kept while their skeletons are
    std::map<std::string, std::unique_ptr<gmo_motion::Skeleton>, std::less<>> skeletons;

    [[nodiscard]] Bytes file(const std::string &path) const {
        auto bytes = read(path);
        if (!bytes) fail("the disc has no " + path);
        return std::move(*bytes);
    }
    [[nodiscard]] Source &source(const std::string &code) {
        auto found = sources.find(code);
        if (found == sources.end())
            found = sources.emplace(code, std::make_unique<Source>(Arc(file("obj/p_" + code + ".objx")))).first;
        return *found->second;
    }
    [[nodiscard]] const gmo_motion::Skeleton &skeleton(const std::string &code) {
        auto found = skeletons.find(code);
        if (found == skeletons.end()) {
            const Bytes &data =
                *skeleton_files.emplace(code, std::make_unique<Bytes>(code == kCode ? model : file("obj/P_" + upper(code) + "_1P.gmo")))
                     .first->second;
            found = skeletons.emplace(code, std::make_unique<gmo_motion::Skeleton>(Span(data))).first;
        }
        return *found->second;
    }
    // An animation of the recipe as a motion chunk named `name`.
    [[nodiscard]] Bytes animation(const Animation &animation, const std::string &name) {
        if (!animation.rebuilt) {
            for (const GmoChunk &chunk : gmo_model(model).motions)
                if (gmo_name(model, chunk) == animation.name) return renamed_motion(Span(model).subspan(chunk.at, chunk.size), name);
            fail("012 Bartz has no animation " + animation.name);
        }
        gmo_motion::Retarget options = animation.retarget;
        options.name = name;
        return gmo_motion::retarget(skeleton(animation.owner), skeleton(std::string(kCode)), options);
    }
};

// ---------------------------------------------------------------------------
// Bringing the original commands into 012's fighter (original_moves.py: bring).

enum class Ref { Hit, Shape, Attack, Projectile, Effect, Sound, Banner, Key };
struct EventRef {
    std::size_t index;
    Ref what;
};
// Event arguments that name things the copy renumbers.
std::vector<EventRef> event_refs(bool ranged, unsigned type) {
    if (ranged) {
        switch (type) {
        case 0x00: return {{0u, Ref::Hit}, {1u, Ref::Shape}};
        case 0x43: return {{0u, Ref::Hit}, {1u, Ref::Shape}, {2u, Ref::Projectile}};
        case 0x7B: return {{0u, Ref::Hit}, {1u, Ref::Shape}};
        case 0x12: return {{0u, Ref::Shape}};
        case 0x4E: return {{0u, Ref::Shape}};
        case 0x20: return {{0u, Ref::Hit}};
        case 0x78: return {{1u, Ref::Hit}};
        case 0x5B: return {{0u, Ref::Hit}};
        case 0x67: return {{1u, Ref::Attack}};
        case 0x79: return {{1u, Ref::Attack}};
        case 0x24: return {{0u, Ref::Projectile}};
        case 0x66: return {{0u, Ref::Effect}};
        case 0x35: return {{0u, Ref::Effect}};
        case 0x2A: return {{2u, Ref::Key}};
        case 0x5F: return {{2u, Ref::Key}};
        default: return {};
        }
    }
    switch (type) {
    case 0x08: return {{0u, Ref::Hit}};
    case 0x04: return {{0u, Ref::Projectile}};
    case 0x29: return {{0u, Ref::Projectile}};
    case 0x2D: return {{0u, Ref::Projectile}, {2u, Ref::Projectile}};
    case 0x33: return {{0u, Ref::Projectile}};
    case 0x19: return {{0u, Ref::Effect}};
    case 0x2F: return {{0u, Ref::Effect}, {2u, Ref::Effect}};
    case 0x30: return {{0u, Ref::Effect}};
    case 0x03: return {{0u, Ref::Sound}};
    case 0x11: return {{1u, Ref::Sound}};
    case 0x12: return {{1u, Ref::Sound}};
    case 0x1D: return {{0u, Ref::Banner}};
    default: return {};
    }
}

constexpr long long kNewMotionOffset = 5000;  // the original's attack motions 10000.. become 15000..
// Hitbox groups of the original's own are stored "parked": record byte 0 = 63
// (a bit nothing sets), byte 1 = the group's number (64..255), and their
// events carry 63 + 256 x group. The port moves the groups of the equipped
// commands onto free ids at battle start (d012_shape_ids.cpp).
constexpr unsigned kParkedShape = 63u, kFirstVirtualShape = 64u;
constexpr long long kFirstAttack = 3100, kFirstHit = 3400, kFirstProjectile = 2300, kFirstFepa = 200;
// Effect records a bone_effect event picks by key bit: weapon trails, glows.
bool keyed_record(std::uint32_t id) { return id == 9999u || id == 8888u; }
// Effects placed as 012 Bartz's own records place them: the flash a weapon
// appears and leaves with.
constexpr std::string_view kPlacedAs012 = "WEP_APPEAR_";
// The original Bartz's commands and the 012 command in the same place of his list.
long long slot_012(long long command) {
    static const std::map<long long, long long> slots = {{408, 162}, {409, 163}, {410, 164}, {411, 165}, {412, 166},
                                                         {413, 167}, {607, 168}, {414, 169}, {415, 170}, {416, 171},
                                                         {604, 172}, {417, 173}, {418, 174}, {419, 175}};
    const auto found = slots.find(command);
    return found == slots.end() ? -1 : found->second;
}
// Follow-ups: the original's ability table says Flare (604) follows Holy (416).
long long follows(long long command) { return command == 604 ? 416 : -1; }

class Merge {
public:
    Merge(Fighter &base, const Recipe &og, Disc &disc, std::vector<std::string> &notes)
        : base_(base), og_(og), disc_(disc), notes_(notes) {}

    // Adds the recipe's commands to the fighter; returns the motion chunks to
    // append to each of its models. `base_anims` is the models' motion count.
    std::vector<Bytes> run(std::size_t base_anims) {
        for (const Bytes &row : og_.attacks) og_attacks_[u16(row, 0u)] = &row;
        for (const Bytes &row : og_.hits) og_hits_[u32(row, 0u)] = &row;
        for (const Magi &row : og_.projectiles) og_magi_[u16(row.head, 0u)] = &row;
        for (const Bytes &row : og_.motions) og_motion_[u16(row, 0u)] = &row;
        for (const Track &track : og_.tracks) og_track_[track.anim] = &track;
        std::map<long long, const Combo *> og_combo;
        for (const Combo &combo : og_.combos) og_combo[combo.command] = &combo;
        const auto combo_of = [&](long long command) -> const Combo & {
            const auto found = og_combo.find(command);
            if (found == og_combo.end()) fail("the recipe has no command " + std::to_string(command));
            return *found->second;
        };

        // 1. What the commands use: attacks (and the follow-ups their events
        // offer), motions, tracks.
        std::set<long long> attacks;
        std::vector<long long> order, motions;
        std::deque<long long> pending;
        for (const auto &[old, command] : og_.commands)
            for (const Bytes &stage : combo_of(old).stages) pending.push_back(u16(stage, 0u));
        while (!pending.empty()) {
            const long long attack = pending.front();
            pending.pop_front();
            if (attacks.contains(attack) || !og_attacks_.contains(attack)) continue;
            attacks.insert(attack);
            order.push_back(attack);
            const Bytes &row = *og_attacks_[attack];
            if (s16(row, 0x0Au) >= 0) pending.push_back(s16(row, 0x0Au));
            for (const long long motion : {static_cast<long long>(u16(row, 0x1Cu)), static_cast<long long>(s16(row, 0x10u))}) {
                if (motion < 10000 || !og_motion_.contains(motion) || std::find(motions.begin(), motions.end(), motion) != motions.end())
                    continue;
                motions.push_back(motion);
                const auto track = og_track_.find(u16(*og_motion_[motion], 2u));
                if (track == og_track_.end()) continue;
                for (const bool ranged : {true, false})
                    for (const Event &event : ranged ? track->second->ranged : track->second->instant)
                        for (const EventRef &ref : event_refs(ranged, event_type(event, ranged)))
                            if (ref.what == Ref::Attack && ref.index < event.args.size()) pending.push_back(event.args[ref.index]);
            }
        }
        std::vector<long long> anims;
        for (const long long motion : motions) {
            const long long anim = u16(*og_motion_[motion], 2u);
            if (std::find(anims.begin(), anims.end(), anim) == anims.end()) anims.push_back(anim);
        }

        std::set<long long> base_motion_ids;
        for (const Bytes &row : base_.motions) base_motion_ids.insert(u16(row, 0u));
        std::map<long long, long long> anim_map, motion_map;
        for (std::size_t k = 0u; k < anims.size(); ++k) anim_map[anims[k]] = static_cast<long long>(base_anims + k);
        for (const long long motion : motions) {
            const long long made = motion + kNewMotionOffset;
            if (base_motion_ids.contains(made) || made > 0xFFFF) fail("motion " + std::to_string(made) + " is taken");
            motion_map[motion] = made;
        }

        // 2. Hits, projectiles, shapes, effects, sounds, banners the tracks and
        // projectiles use.
        std::set<long long> used_attacks;
        for (const Bytes &row : base_.attacks) used_attacks.insert(u16(row, 0u));
        for (const long long attack : order) attack_map_[attack] = allocate(kFirstAttack, used_attacks);
        for (const Bytes &row : base_.hits) {
            base_hits_[u32(row, 0u)] = row;
            used_hits_.insert(u32(row, 0u));
        }
        std::vector<std::pair<unsigned, std::vector<Bytes>>> base_groups;  // in the order met
        for (const Bytes &row : base_.shapes) {
            if (row[0] == kParkedShape)
                fail("hitbox id " + std::to_string(kParkedShape) + " is in use; it is needed for the original's groups");
            auto found = std::find_if(base_groups.begin(), base_groups.end(), [&](const auto &g) { return g.first == row[0]; });
            if (found == base_groups.end()) found = base_groups.insert(base_groups.end(), {row[0], {}});
            found->second.emplace_back(row.begin() + 1, row.end());
        }
        for (auto &[id, rows] : base_groups) {
            std::sort(rows.begin(), rows.end());
            by_content_[rows] = id;
        }
        for (const Bytes &row : og_.shapes) og_groups_[row[0]].push_back(row);

        const std::vector<std::string> names = base_.fep.names();
        for (std::size_t i = 0u; i < names.size(); ++i) base_names_[names[i]] = i;
        own_cue_ = base_.bank.number() * 100u;
        for (const Bytes &row : base_.fepa) {
            used_fepa_.insert(u32(row, 0x24u));
            own_rows_.push_back(row);
            const std::uint64_t group_mask = u32(row, 0x3Cu) | std::uint64_t(u32(row, 0x40u)) << 32u;
            for (unsigned b = 0u; b < 64u; ++b)
                if ((group_mask >> b & 1u) != 0u) used_bits_.insert(b);
            // Weapon trails and glows: a bone_effect event takes the first
            // record of that id with its key bit (+0x44). His own records
            // come first, so the original's get bits his leave free.
            if (keyed_record(u32(row, 0x24u)))
                for (unsigned b = 0u; b < 32u; ++b)
                    if ((u32(row, 0x44u) >> b & 1u) != 0u) key_used_[u32(row, 0x24u)].insert(b);
        }
        for (const long long anim : anims) renames_[og_.animation(anim).name] = og_.animation(anim).name + "_OG";
        bank_base_ = static_cast<long long>(base_.bank.number()) * 100;
        for (const Magi &row : base_.projectiles.rows) used_magi_.insert(u16(row.head, 0u));

        // 3. Attack records.
        for (const long long old : order) {
            Bytes row = *og_attacks_[old];
            put16(row, 0u, static_cast<std::uint32_t>(attack_map_[old]));
            if (s16(row, 0x0Au) >= 0 && attack_map_.contains(s16(row, 0x0Au)))
                put16(row, 0x0Au, static_cast<std::uint32_t>(attack_map_[s16(row, 0x0Au)]));
            for (const std::size_t at : {std::size_t(0x10u), std::size_t(0x1Cu), std::size_t(0x1Eu)})
                if (const auto found = motion_map.find(s16(row, at)); found != motion_map.end())
                    put16(row, at, static_cast<std::uint32_t>(found->second));
            if (row[0x21] != 0xFFu) {
                const long long made = banner(row[0x21]);
                if (made > 0xFF) fail("too many banners");
                row[0x21] = static_cast<std::uint8_t>(made);
            }
            base_.attacks.push_back(std::move(row));
        }

        // 4. Motions, tracks and the models' animations.
        for (const long long motion : motions) {
            Bytes row = *og_motion_[motion];
            put16(row, 0u, static_cast<std::uint32_t>(motion_map[motion]));
            put16(row, 2u, static_cast<std::uint32_t>(anim_map[u16(row, 2u)]));
            base_.motions.push_back(std::move(row));
        }
        std::vector<Bytes> chunks;
        for (const long long anim : anims) {
            const Animation &animation = og_.animation(anim);
            const std::string name = animation.name + "_OG";
            chunks.push_back(disc_.animation(animation, name));
            for (std::size_t index = 0u; index < og_.fepa.size(); ++index) {  // records that start with the animation
                const std::string bound = text_at(og_.fepa[index], 0xC8u, 32u);
                if (!bound.empty() && bound == animation.name) bring_row(index, name);
            }
            const auto source = og_track_.find(static_cast<std::uint32_t>(anim));
            if (source == og_track_.end()) continue;
            Track track;
            track.anim = static_cast<std::uint32_t>(anim_map[anim]);
            for (const bool ranged : {true, false}) {
                for (const Event &event : ranged ? source->second->ranged : source->second->instant) {
                    std::vector<std::uint32_t> args = event.args;
                    bool keep = true;
                    for (const EventRef &ref : event_refs(ranged, event_type(event, ranged))) {
                        if (ref.index >= args.size()) continue;
                        const std::uint32_t value = args[ref.index];
                        switch (ref.what) {
                        case Ref::Hit: args[ref.index] = static_cast<std::uint32_t>(hit(value)); break;
                        case Ref::Shape: args[ref.index] = static_cast<std::uint32_t>(shape(value)); break;
                        case Ref::Attack:
                            if (const auto found = attack_map_.find(value); found != attack_map_.end())
                                args[ref.index] = static_cast<std::uint32_t>(found->second);
                            break;
                        case Ref::Projectile: args[ref.index] = static_cast<std::uint32_t>(projectile(value)); break;
                        case Ref::Effect:
                            if (const auto made = effect(value)) args[ref.index] = static_cast<std::uint32_t>(*made);
                            else keep = false;
                            break;
                        case Ref::Sound:
                            if (const auto made = sound(value)) args[ref.index] = static_cast<std::uint32_t>(*made);
                            else keep = false;
                            break;
                        case Ref::Banner: args[ref.index] = static_cast<std::uint32_t>(banner(value)); break;
                        case Ref::Key:
                            if (keyed_record(args[1])) {
                                std::vector<std::size_t> rows;
                                for (std::size_t i = 0u; i < og_.fepa.size(); ++i) {
                                    const Bytes &r = og_.fepa[i];
                                    if (u32(r, 0x24u) == args[1] && value < 32u && (u32(r, 0x44u) >> value & 1u) != 0u &&
                                        u32(r, 0x34u) == 0u)
                                        rows.push_back(i);
                                }
                                bool all = true;
                                for (const std::size_t i : rows) all = bring_row(i, std::nullopt) && all;
                                if (all && !rows.empty()) args[ref.index] = key_bit(args[1], value);
                            }
                            break;
                        }
                    }
                    if (keep) (ranged ? track.ranged : track.instant).push_back({event.head, std::move(args)});
                    else notes_.push_back("an event of animation " + std::to_string(anim) + " was left out: its effect or sound did not come");
                }
            }
            base_.tracks.tracks.push_back(std::move(track));
        }

        // 5. Commands and CPU rows.
        for (const auto &[old, command] : og_.commands) {
            Combo combo;
            combo.command = static_cast<int>(command);
            for (const Bytes &stage : combo_of(old).stages) {
                Bytes copy = stage;
                if (const auto found = attack_map_.find(u16(stage, 0u)); found != attack_map_.end())
                    put16(copy, 0u, static_cast<std::uint32_t>(found->second));
                combo.stages.push_back(std::move(copy));
            }
            if (const long long first = follows(old); first >= 0)
                for (const auto &[other, other_command] : og_.commands)
                    if (other == first) combo.prerequisite = static_cast<int>(other_command);
            base_.combos.push_back(std::move(combo));
        }
        for (const auto &[old, command] : og_.commands) {
            const long long like = slot_012(old);
            const auto row = std::find_if(base_.brat.rows.begin(), base_.brat.rows.end(),
                                          [&](const Brat::Row &r) { return like >= 0 && r.command == std::uint32_t(like); });
            if (row == base_.brat.rows.end()) continue;
            Brat::Row made;
            made.command = static_cast<std::uint32_t>(command);
            const auto situations = row->situations;
            for (std::size_t k = 0u; k < 6u; ++k) {
                const Bytes copy = lenient(base_.brat.pool, situations[k].first, situations[k].second);
                made.situations[k] = {static_cast<std::uint32_t>(base_.brat.pool.size()), situations[k].second};
                add(base_.brat.pool, copy);
            }
            base_.brat.rows.push_back(made);
        }
        std::size_t copied_hits = 0u;
        for (const auto &[old, made] : hit_map_) copied_hits += old != made ? 1u : 0u;
        notes_.push_back(std::to_string(order.size()) + " attacks, " + std::to_string(motions.size()) + " motions on " +
                         std::to_string(anims.size()) + " new animations, " + std::to_string(magi_map_.size()) +
                         " projectiles, " + std::to_string(copied_hits) + " hits copied");
        return chunks;
    }

private:
    // The next free id from `first` up; ids of 8000 and more are the game's
    // shared ones and do not count.
    static long long allocate(long long first, std::set<long long> &used) {
        long long value = first - 1;
        for (const long long u : used)
            if (u < 8000) value = std::max(value, u);
        ++value;
        used.insert(value);
        return value;
    }

    long long hit(long long old) {
        if (const auto found = hit_map_.find(old); found != hit_map_.end()) return found->second;
        const auto source = og_hits_.find(old);
        const auto mine = base_hits_.find(old);
        if (source == og_hits_.end()) return hit_map_[old] = old;  // a shared hit the game finds in battle.bin
        if (mine != base_hits_.end() && mine->second == *source->second) return hit_map_[old] = old;
        const long long made = allocate(kFirstHit, used_hits_);
        Bytes row = *source->second;
        put32(row, 0u, static_cast<std::uint32_t>(made));
        if (row[0x0C] != 0xFFu && row[0x0C] < 64u) row[0x0C] = static_cast<std::uint8_t>(group(row[0x0C]));  // its contact effect
        base_.hits.push_back(std::move(row));
        return hit_map_[old] = made;
    }

    // A hitbox group of the original: the 012 id of a group with the same
    // spheres, else a group of its own, parked. Returns what its events carry.
    long long shape(long long old) {
        if (const auto found = shape_map_.find(old); found != shape_map_.end()) return found->second;
        const auto rows = old >= 0 && old <= 0xFF ? og_groups_.find(static_cast<unsigned>(old)) : og_groups_.end();
        if (rows == og_groups_.end() || rows->second.empty()) return shape_map_[old] = old;
        std::vector<Bytes> key;
        for (const Bytes &row : rows->second) key.emplace_back(row.begin() + 1, row.end());
        std::sort(key.begin(), key.end());
        if (const auto same = by_content_.find(key); same != by_content_.end()) return shape_map_[old] = same->second;
        const unsigned made = next_group_;
        if (made > 0xFFu) fail("more than 192 hitbox groups of the original's");
        ++next_group_;
        for (const Bytes &row : rows->second) {
            Bytes copy = row;
            copy[0] = static_cast<std::uint8_t>(kParkedShape);
            copy[1] = static_cast<std::uint8_t>(made);
            base_.shapes.push_back(std::move(copy));
        }
        by_content_[key] = kParkedShape + 256ll * made;
        ++parked_groups_;
        return shape_map_[old] = kParkedShape + 256ll * made;
    }

    // The name index of effect `name` in 012 Bartz's package, copying it in
    // (with its sounds) from the 012 fighter the recipe names when he lacks it.
    std::size_t ensure_effect(const std::string &name) {
        if (const auto found = base_names_.find(name); found != base_names_.end()) return found->second;
        if (base_.fxse.size() != base_names_.size()) fail("the effect package and its sound records disagree");
        const auto from = og_.effect_from.find(name);
        if (from == og_.effect_from.end()) fail("effect " + name + ": the recipe names no fighter that has it");
        Source &source = disc_.source(from->second.first);
        const auto there = source.by_stem.find(effect_stem(from->second.second));
        if (there == source.by_stem.end()) fail("effect " + name + ": " + from->second.first + " has no " + from->second.second);
        const std::size_t made = effect_package::copy_effect(base_.fep, source.fep, there->second.second, name);
        if (there->second.first >= source.fxse.size()) fail("effect " + name + ": it has no sound record");
        base_.fxse.push_back(fxse_moved(source.fxse[there->second.first], source.bank));
        base_names_[name] = made;
        return made;
    }
    // An effect's sound record (16 x {u32 bank x 100, u32 slot, 3 x u32}) for
    // Bartz: cues on the source fighter's bank are copied into his.
    Bytes fxse_moved(Bytes row, const SoundBank &source_bank) {
        for (std::size_t k = 0u; k < 16u; ++k) {
            const std::size_t at = 20u * k;
            const std::uint32_t cue_bank = u32(row, at), slot = u32(row, at + 4u);
            if (cue_bank == 0u || std::uint64_t(cue_bank) != std::uint64_t(source_bank.number()) * 100u) continue;
            if (const auto made = bring_sound(base_.bank, source_bank, slot)) {
                put32(row, at, own_cue_);
                put32(row, at + 4u, *made);
            } else {
                std::fill(row.begin() + static_cast<std::ptrdiff_t>(at), row.begin() + static_cast<std::ptrdiff_t>(at + 20u), std::uint8_t{0});
                notes_.push_back("an effect's sound could not come");
            }
        }
        return row;
    }
    unsigned key_bit(std::uint32_t record, unsigned old) {
        const auto key = std::pair{record, old};
        if (const auto found = key_map_.find(key); found != key_map_.end()) return found->second;
        std::set<unsigned> &used = key_used_[record];
        for (unsigned b = 0u; b < 32u; ++b)
            if (!used.contains(b)) {
                used.insert(b);
                return key_map_[key] = b;
            }
        fail("no free key bit for effect record " + std::to_string(record));
    }
    unsigned bit(unsigned old) {
        if (const auto found = bit_map_.find(old); found != bit_map_.end()) return found->second;
        for (unsigned b = 0u; b < 64u; ++b)
            if (!used_bits_.contains(b)) {
                used_bits_.insert(b);
                return bit_map_[old] = b;
            }
        fail("no free effect group bit");
    }
    // Copies the recipe's placement record `index`: its effect by name, a new
    // id per original id, its group bits moved to bits of their own.
    bool bring_row(std::size_t index, const std::optional<std::string> &motion_name) {
        if (brought_rows_.contains(index)) return true;
        const Bytes &old_row = og_.fepa[index];
        if (old_row.size() != 260u) fail("the recipe has an effect record of another size");
        const auto effect_name = og_.effect_names.find(u32(old_row, 0x28u));
        if (effect_name == og_.effect_names.end()) {
            notes_.push_back("an effect record names no effect");
            return false;
        }
        Bytes row = old_row;
        put32(row, 0x28u, static_cast<std::uint32_t>(ensure_effect(effect_name->second)));
        if (effect_stem(effect_name->second).starts_with(kPlacedAs012)) {
            // Placed and sized as 012 Bartz's own record of it on the same
            // weapon bone (the size alone when he has none on that bone).
            const Bytes *mine = nullptr, *same_bone = nullptr;
            for (const Bytes &r : own_rows_) {
                if (u32(r, 0x28u) != u32(row, 0x28u)) continue;
                if (mine == nullptr) mine = &r;
                if (same_bone == nullptr && std::equal(r.begin() + 0xA8, r.begin() + 0xC8, row.begin() + 0xA8)) same_bone = &r;
            }
            if (same_bone != nullptr) std::copy(same_bone->begin(), same_bone->begin() + 0x24, row.begin());
            else if (mine != nullptr) std::copy(mine->begin() + 0x18, mine->begin() + 0x24, row.begin() + 0x18);
        }
        const std::uint64_t group_mask = u32(row, 0x3Cu) | std::uint64_t(u32(row, 0x40u)) << 32u;
        std::uint64_t new_mask = 0u;
        for (unsigned b = 0u; b < 64u; ++b)
            if ((group_mask >> b & 1u) != 0u) new_mask |= std::uint64_t(1u) << bit(b);
        put32(row, 0x3Cu, static_cast<std::uint32_t>(new_mask));
        put32(row, 0x40u, static_cast<std::uint32_t>(new_mask >> 32u));
        std::optional<std::string> bound = motion_name;
        if (!bound || bound->empty()) {
            bound.reset();
            if (const auto renamed = renames_.find(text_at(row, 0xC8u, 32u)); renamed != renames_.end()) bound = renamed->second;
        }
        if (bound) {
            std::fill(row.begin() + 0xC8, row.begin() + 0xE8, std::uint8_t{0});
            std::copy_n(bound->begin(), std::min<std::size_t>(bound->size(), 32u), row.begin() + 0xC8);
        }
        const std::uint32_t old_id = u32(old_row, 0x24u);
        if (keyed_record(old_id)) {
            std::uint32_t keys = 0u;
            for (unsigned b = 0u; b < 32u; ++b)
                if ((u32(row, 0x44u) >> b & 1u) != 0u) keys |= 1u << key_bit(old_id, b);
            put32(row, 0x44u, keys);
        }
        if (!fepa_map_.contains(old_id)) fepa_map_[old_id] = old_id >= 8000u ? old_id : allocate(kFirstFepa, used_fepa_);
        put32(row, 0x24u, static_cast<std::uint32_t>(fepa_map_[old_id]));
        base_.fepa.push_back(std::move(row));
        brought_rows_.insert(index);
        return true;
    }
    // The new id of the recipe's placement record id `old`; nothing when none
    // of its records came.
    std::optional<long long> effect(std::uint32_t old) {
        bool any = false, found = false;
        for (std::size_t i = 0u; i < og_.fepa.size(); ++i) {
            if (u32(og_.fepa[i], 0x24u) != old) continue;
            found = true;
            any = bring_row(i, std::nullopt) || any;
        }
        if (!found) notes_.push_back("effect record " + std::to_string(old) + " is not in the recipe");
        if (!any) return std::nullopt;
        return fepa_map_.at(old);
    }
    // An effect group bit of the original (a projectile's look, a hit's
    // contact effect): every record with the bit comes along once, and the
    // bit is renumbered.
    unsigned group(unsigned old) {
        if (!groups_done_.contains(old)) {
            groups_done_.insert(old);
            for (std::size_t i = 0u; i < og_.fepa.size(); ++i) {
                const Bytes &r = og_.fepa[i];
                const std::uint64_t group_mask = u32(r, 0x3Cu) | std::uint64_t(u32(r, 0x40u)) << 32u;
                if ((group_mask >> old & 1u) != 0u && u32(r, 0x34u) == 0u) bring_row(i, std::nullopt);
            }
        }
        return bit(old);
    }

    // A move's sound: his own slot, or the clip the recipe names of another
    // 012 fighter, put into an empty slot of his bank.
    std::optional<long long> sound(long long old) {
        if (const auto found = sound_map_.find(old); found != sound_map_.end()) return found->second;
        const auto plan = og_.sounds.find(old - bank_base_);
        if (plan == og_.sounds.end() || !plan->second) return sound_map_[old] = old;
        const auto made = bring_sound(base_.bank, disc_.source(plan->second->first).bank, static_cast<std::uint64_t>(plan->second->second));
        if (!made) {
            notes_.push_back("sound slot " + std::to_string(old - bank_base_) + " could not come");
            return sound_map_[old] = std::nullopt;
        }
        return sound_map_[old] = bank_base_ + *made;
    }

    long long banner(long long old) {
        if (const auto found = banner_map_.find(old); found != banner_map_.end()) return found->second;
        if (og_.banners.empty() || base_.banners.empty()) fail("the banner texts have no table");
        const Strings &first = og_.banners[0];
        if (old == 0xFF || old < 0 || static_cast<std::size_t>(old) >= first.size()) return banner_map_[old] = old;
        const long long made = static_cast<long long>(base_.banners[0].size());
        for (std::size_t k = 0u; k < base_.banners.size(); ++k) {
            const Strings &source = og_.banners[k < og_.banners.size() ? k : 0u];
            base_.banners[k].push_back(static_cast<std::size_t>(old) < source.size() ? source[static_cast<std::size_t>(old)]
                                                                                    : first[static_cast<std::size_t>(old)]);
        }
        return banner_map_[old] = made;
    }

    long long projectile(long long old) {
        if (const auto found = magi_map_.find(old); found != magi_map_.end()) return found->second;
        const auto source = og_magi_.find(old);
        if (source == og_magi_.end()) {
            notes_.push_back("projectile " + std::to_string(old) + " is not in the recipe");
            return magi_map_[old] = old;
        }
        const long long made = allocate(kFirstProjectile, used_magi_);
        magi_map_[old] = made;
        Magi row;
        row.head = source->second->head;
        row.values = source->second->values;
        put16(row.head, 0u, static_cast<std::uint32_t>(made));
        for (std::size_t at = 0x0Eu; at < 0x1Eu; at += 2u)  // links: emit, terrain x5, hit, fall, clash
            if (const int link = s16(row.head, at); link >= 0) put16(row.head, at, static_cast<std::uint32_t>(projectile(link)));
        for (const std::size_t at : {std::size_t(0x0Au), std::size_t(0x0Cu)})
            if (u16(row.head, at) != 0xFFFFu) put16(row.head, at, static_cast<std::uint32_t>(hit(u16(row.head, at))));
        if (const unsigned look = u16(row.head, 0x04u); look != 0xFFFFu && look < 64u) put16(row.head, 0x04u, group(look));
        if (u16(row.head, 0x1Eu) == 6u)  // starts at an effect record
            if (const auto target = effect(u16(row.head, 0x20u))) put16(row.head, 0x20u, static_cast<std::uint32_t>(*target));
        base_.projectiles.rows.push_back(std::move(row));
        return made;
    }

    Fighter &base_;
    const Recipe &og_;
    Disc &disc_;
    std::vector<std::string> &notes_;

    std::map<long long, const Bytes *> og_attacks_, og_hits_, og_motion_;
    std::map<long long, const Magi *> og_magi_;
    std::map<std::uint32_t, const Track *> og_track_;
    std::map<long long, long long> attack_map_, hit_map_, shape_map_, magi_map_, banner_map_, fepa_map_;
    std::map<long long, Bytes> base_hits_;
    std::set<long long> used_hits_, used_magi_, used_fepa_;
    std::map<std::vector<Bytes>, long long> by_content_;  // a hitbox group's spheres -> what events carry
    std::map<unsigned, std::vector<Bytes>> og_groups_;
    unsigned next_group_{kFirstVirtualShape};
    std::size_t parked_groups_{};
    std::map<std::string, std::size_t> base_names_;
    std::uint32_t own_cue_{};
    std::vector<Bytes> own_rows_;
    std::set<unsigned> used_bits_, groups_done_;
    std::map<unsigned, unsigned> bit_map_;
    std::map<std::uint32_t, std::set<unsigned>> key_used_;
    std::map<std::pair<std::uint32_t, unsigned>, unsigned> key_map_;
    std::set<std::size_t> brought_rows_;
    std::map<std::string, std::string> renames_;
    long long bank_base_{};
    std::map<long long, std::optional<long long>> sound_map_;
};

// ---------------------------------------------------------------------------
// Abilities: system.bin's ablt / chrl / amas, the names and the help texts.

// A message table: plain (u16 count, u16 offsets in words) or OFF4 ("OFF4",
// u32 count, u32 offsets in 4-byte units); the text is hidden by its index.
struct Messages {
    std::vector<std::optional<Units>> strings;
    bool off4{};
};
Messages read_messages(Span data) {
    Messages out;
    out.off4 = starts(data, "OFF4");
    const std::size_t count = out.off4 ? u32(data, 4u) : u16(data, 0u);
    for (std::size_t index = 0u; index < count; ++index) {
        std::size_t at = 0u, size = 0u, start = 0u;
        if (out.off4) {
            at = std::size_t(u32(data, 8u + 4u * index)) * 4u;
            if (at != 0u) size = u32(data, at), start = at + 4u;
        } else {
            at = std::size_t(u16(data, 2u + 2u * index)) * 2u;
            if (at != 0u) size = u16(data, at), start = at + 2u;
        }
        if (at == 0u) {
            out.strings.emplace_back();
            continue;
        }
        Units units;
        for (std::size_t k = 0u; k < size / 2u; ++k)
            units.push_back(static_cast<std::uint16_t>(u16(data, start + 2u * k) ^ kTextKey[(index + k) & 15u]));
        out.strings.emplace_back(std::move(units));
    }
    return out;
}
Bytes write_messages(const Messages &messages, bool off4) {
    const auto hide = [](Bytes &out, const Units &units, std::size_t index) {
        for (std::size_t k = 0u; k < units.size(); ++k) add16(out, static_cast<std::uint32_t>(units[k] ^ kTextKey[(index + k) & 15u]));
    };
    const auto &strings = messages.strings;
    if (!off4) {
        const std::size_t start = 2u + 2u * strings.size();
        std::vector<std::size_t> offsets;
        Bytes body;
        for (std::size_t index = 0u; index < strings.size(); ++index) {
            if (!strings[index]) {
                offsets.push_back(0u);
                continue;
            }
            const std::size_t position = start + body.size();
            if (position / 2u > 0xFFFFu) return write_messages(messages, true);
            offsets.push_back(position / 2u);
            add16(body, static_cast<std::uint32_t>(2u * strings[index]->size()));
            hide(body, *strings[index], index);
        }
        Bytes out;
        add16(out, static_cast<std::uint32_t>(strings.size()));
        for (const std::size_t offset : offsets) add16(out, static_cast<std::uint32_t>(offset));
        add(out, body);
        return out;
    }
    std::size_t start = 8u + 4u * strings.size();
    start += (4u - start % 4u) % 4u;
    std::vector<std::size_t> offsets;
    Bytes body;
    for (std::size_t index = 0u; index < strings.size(); ++index) {
        if (!strings[index]) {
            offsets.push_back(0u);
            continue;
        }
        offsets.push_back((start + body.size()) / 4u);
        add32(body, static_cast<std::uint32_t>(2u * strings[index]->size()));
        hide(body, *strings[index], index);
        pad(body, 4u);
    }
    Bytes out;
    add(out, "OFF4");
    add32(out, static_cast<std::uint32_t>(strings.size()));
    for (const std::size_t offset : offsets) add32(out, static_cast<std::uint32_t>(offset));
    out.resize(start);
    add(out, body);
    return out;
}

// "mess pak": u16 count at +10, {u32 offset, u32 size} from +16, the entries
// on 16 bytes.
std::vector<Bytes> read_mess_pak(Span data) {
    if (!starts(data, "mess pak")) fail("not a mess pak");
    std::vector<Bytes> entries;
    for (std::size_t i = 0u; i < u16(data, 10u); ++i) entries.push_back(lenient(data, u32(data, 16u + 8u * i), u32(data, 20u + 8u * i)));
    return entries;
}
Bytes write_mess_pak(const std::vector<Bytes> &entries) {
    const std::size_t head = 16u + 8u * entries.size();
    std::size_t position = head + (16u - head % 16u) % 16u;
    Bytes table, body;
    for (const Bytes &entry : entries) {
        add32(table, static_cast<std::uint32_t>(position));
        add32(table, static_cast<std::uint32_t>(entry.size()));
        add(body, entry);
        pad(body, 16u);
        position += entry.size() + (16u - entry.size() % 16u) % 16u;
    }
    Bytes out;
    add(out, "mess pak");
    add16(out, 1u);
    add16(out, static_cast<std::uint32_t>(entries.size()));
    add32(out, 0u);
    add(out, table);
    pad(out, 16u);
    add(out, body);
    return out;
}

// chrl (what a level gives) and amas (what mastering gives): a 16-byte head
// with the row count at +8, rows {u32 character, u32 count, u32 offset, u32
// 0}, then 8-byte entries.
struct LevelTable {
    Bytes head;
    std::map<std::uint32_t, std::vector<Bytes>> rows;  // by character
};
LevelTable read_levels(Span data) {
    LevelTable table;
    table.head = slice(data, 0u, 16u);
    for (std::size_t i = 0u; i < u32(data, 8u); ++i) {
        const std::size_t at = 16u + 16u * i;
        std::vector<Bytes> entries;
        for (std::size_t k = 0u; k < u32(data, at + 4u); ++k) entries.push_back(slice(data, u32(data, at + 8u) + 8u * k, 8u));
        table.rows[u32(data, at)] = std::move(entries);
    }
    return table;
}
Bytes write_levels(const LevelTable &table) {
    Bytes out = table.head;
    put32(out, 8u, static_cast<std::uint32_t>(table.rows.size()));
    const std::size_t position = 16u + 16u * table.rows.size();
    Bytes index, body;
    for (const auto &[character, entries] : table.rows) {
        add32(index, character);
        add32(index, static_cast<std::uint32_t>(entries.size()));
        add32(index, static_cast<std::uint32_t>(position + body.size()));
        add32(index, 0u);
        for (const Bytes &entry : entries) add(body, entry);
    }
    add(out, index);
    add(out, body);
    return out;
}
// Puts `ability` in the chrl entry of `level`: a free slot of an entry, or a
// new entry.
void add_level(std::vector<Bytes> &entries, int level, std::uint32_t ability) {
    for (Bytes &entry : entries)
        if (entry[0] == level)
            for (const std::size_t at : {std::size_t(2u), std::size_t(4u), std::size_t(6u)})
                if (u16(entry, at) == 0xFFFFu) {
                    put16(entry, at, ability);
                    return;
                }
    Bytes entry = {static_cast<std::uint8_t>(level), 0u};
    add16(entry, ability);
    add16(entry, 0xFFFFu);
    add16(entry, 0xFFFFu);
    entries.push_back(std::move(entry));
    std::stable_sort(entries.begin(), entries.end(), [](const Bytes &a, const Bytes &b) { return a[0] < b[0]; });
}

constexpr std::uint32_t kFirstAbilityKey = 0x20u;  // ability ids (character << 8) | key; 012's Bartz uses 0x01..0x0F
// CP of every (OG) ability, and once mastered: the owner's choice.
constexpr std::uint32_t kAbilityCp = 30u, kAbilityCpMastered = 15u;

// Gives Bartz an "(OG)" ability per command. `files` holds the disc's ability
// files by path and is changed in place. Returns (ability id, learn level).
std::vector<std::pair<std::uint16_t, int>> add_abilities(std::vector<File> &files, const Recipe &recipe,
                                                         std::vector<std::string> &notes) {
    const auto file = [&](std::string_view path) -> Bytes * {
        for (File &f : files)
            if (f.path == path) return &f.bytes;
        return nullptr;
    };
    Bytes *main_bin = file(kMainBin);
    if (main_bin == nullptr) fail("the disc has no " + std::string(kMainBin));
    Mpk system_main(*main_bin);
    Arc system(system_main.get("system.bin"));
    const Bytes ablt = system.get("ablt");
    std::vector<Bytes> rows;
    for (std::size_t i = 0u; i < u32(ablt, 8u); ++i) rows.push_back(slice(ablt, 16u + 40u * i, 40u));
    std::set<std::uint32_t> taken, sets;
    for (const Bytes &row : rows) taken.insert(u16(row, 0u));
    const Bytes &fncs = system.get("fncs");
    for (std::size_t i = 0u; i < u16(fncs, 8u); ++i) sets.insert(u16(fncs, 16u + 8u * i));

    struct Language {
        std::string code;
        Mpk names;
        Messages name_table;
        bool has_help{};
        std::vector<Bytes> help_pack;
        Messages help_table;
    };
    std::vector<Language> languages;
    for (const std::string_view code : kLanguages) {
        Bytes *names = file(names_archive(code));
        if (names == nullptr) continue;
        Language language{std::string(code), Mpk(*names), {}, false, {}, {}};
        language.name_table = read_messages(language.names.get("ability.bin"));
        if (Bytes *help = file(help_pack(code))) {
            language.has_help = true;
            language.help_pack = read_mess_pak(*help);
            if (language.help_pack.empty()) fail("a help pack has no entry");
            language.help_table = read_messages(language.help_pack[0]);
        }
        languages.push_back(std::move(language));
    }
    if (languages.empty()) fail("the disc has no ability names");
    const bool any_help = std::any_of(languages.begin(), languages.end(), [](const Language &l) { return l.has_help; });
    const auto pick = [](const std::vector<std::pair<std::string, Units>> &texts, const std::string &code) {
        const Units *english = nullptr;
        for (const auto &[language, units] : texts) {
            if (language == code && !units.empty()) return units;
            if (language == "EN" && !units.empty()) english = &units;
        }
        return english != nullptr ? *english : Units();
    };
    const auto source_of = [&](long long command) -> const AbilitySource & {
        for (const AbilitySource &source : recipe.abilities)
            if (source.command == command) return source;
        fail("the recipe has no ability for command " + std::to_string(command));
    };

    std::vector<std::pair<long long, std::uint32_t>> new_ids;  // original command -> ability, in order
    const auto new_id = [&](long long command) -> std::optional<std::uint32_t> {
        for (const auto &[old, ability] : new_ids)
            if (old == command) return ability;
        return std::nullopt;
    };
    for (std::size_t key = 0u; key < recipe.commands.size(); ++key) {
        const auto &[old, command] = recipe.commands[key];
        const AbilitySource &source = source_of(old);
        const std::uint32_t ability = kCharId << 8u | (kFirstAbilityKey + static_cast<std::uint32_t>(key));
        if (taken.contains(ability)) fail("ability " + std::to_string(ability) + " is taken");
        new_ids.emplace_back(old, ability);
        Bytes row = source.row;
        put16(row, 0x00u, ability);
        put16(row, 0x0Cu, static_cast<std::uint32_t>(command));
        put16(row, 0x08u, 0xFFFFu);
        put16(row, 0x0Eu, kAbilityCp);
        put16(row, 0x10u, kAbilityCpMastered);
        if (u16(row, 0x14u) != 0xFFFFu && !sets.contains(u16(row, 0x14u))) {
            notes.push_back("an equip bonus set is not in 012's table, left out");
            put16(row, 0x14u, 0xFFFFu);
        }
        std::size_t name_index = 0u;
        for (const Language &language : languages) name_index = std::max(name_index, language.name_table.strings.size());
        for (Language &language : languages) {
            Units text = pick(source.names, language.code);
            for (const char c : std::string_view(" (OG)")) text.push_back(static_cast<std::uint16_t>(c));
            text.push_back(0u);
            language.name_table.strings.resize(name_index);
            language.name_table.strings.emplace_back(std::move(text));
        }
        put16(row, 0x02u, static_cast<std::uint32_t>(name_index));
        if (any_help) {
            std::size_t help_index = 0u;
            for (const Language &language : languages)
                if (language.has_help) help_index = std::max(help_index, language.help_table.strings.size());
            for (Language &language : languages) {
                if (!language.has_help) continue;
                Units text = pick(source.helps, language.code);
                text.push_back(0u);
                language.help_table.strings.resize(help_index);
                language.help_table.strings.emplace_back(std::move(text));
            }
            put16(row, 0x04u, static_cast<std::uint32_t>(help_index));
        }
        rows.push_back(std::move(row));
    }
    for (const auto &[old, ability] : new_ids)
        if (const long long first = follows(old); first >= 0)
            if (const auto other = new_id(first))
                for (Bytes &row : rows)
                    if (u16(row, 0u) == ability) {
                        put16(row, 0x06u, *other);
                        break;
                    }

    std::stable_sort(rows.begin(), rows.end(), [](const Bytes &a, const Bytes &b) { return u16(a, 0u) < u16(b, 0u); });
    if (rows.size() > 808u) fail(std::to_string(rows.size()) + " ability records; the game's loader holds 808");
    Bytes new_ablt = slice(ablt, 0u, 16u);
    put32(new_ablt, 8u, static_cast<std::uint32_t>(rows.size()));
    for (const Bytes &row : rows) add(new_ablt, row);
    system.get("ablt") = std::move(new_ablt);

    // Learn levels as in the original; an ability it gives from the start is
    // not in its table (the port gives those). Mastery unlocks likewise.
    std::map<std::uint32_t, int> learn;
    for (const auto &[old, ability] : new_ids) learn[ability] = 1;
    LevelTable levels = read_levels(system.get("chrl"));
    std::vector<std::pair<long long, std::uint32_t>> by_level = new_ids;
    std::stable_sort(by_level.begin(), by_level.end(), [&](const auto &a, const auto &b) {
        const int level_a = source_of(a.first).level.value_or(0), level_b = source_of(b.first).level.value_or(0);
        return level_a != level_b ? level_a < level_b : a.second < b.second;
    });
    for (const auto &[old, ability] : by_level)
        if (const auto level = source_of(old).level) {
            add_level(levels.rows[kCharId], *level, ability);
            learn[ability] = *level;
        }
    system.get("chrl") = write_levels(levels);
    LevelTable unlocks = read_levels(system.get("amas"));
    for (const auto &[old, ability] : new_ids) {
        const auto &needs = source_of(old).mastery;
        if (!needs || needs->empty() || !std::all_of(needs->begin(), needs->end(), [&](long long n) { return new_id(n).has_value(); }))
            continue;
        if (needs->size() > 2u) fail("an ability follows more than two others");
        Bytes row;
        add16(row, ability);
        add16(row, static_cast<std::uint32_t>(needs->size()));
        for (std::size_t k = 0u; k < 2u; ++k) add16(row, k < needs->size() ? *new_id((*needs)[k]) : 0xFFFFu);
        unlocks.rows[kCharId].push_back(std::move(row));
        learn[ability] = 0;
    }
    system.get("amas") = write_levels(unlocks);

    system_main.get("system.bin") = system.to_bytes();
    *main_bin = system_main.to_bytes();
    for (Language &language : languages) {
        language.names.get("ability.bin") = write_messages(language.name_table, language.name_table.off4);
        *file(names_archive(language.code)) = language.names.to_bytes();
        if (!language.has_help) continue;
        language.help_pack[0] = write_messages(language.help_table, language.help_table.off4);
        *file(help_pack(language.code)) = write_mess_pak(language.help_pack);
    }
    std::vector<std::pair<std::uint16_t, int>> out;
    for (const auto &[old, ability] : new_ids) out.emplace_back(static_cast<std::uint16_t>(ability), learn[ability]);
    return out;
}

std::vector<Mark> marks(const json::Value &recipe, std::string_view which) {
    std::vector<Mark> out;
    for (const auto &[path, mark] : member(member(recipe, "check"), which).members) {
        if (mark.items.size() != 2u) fail("the recipe's check block is not sizes and CRCs");
        Mark entry;
        entry.path = path;
        entry.size = static_cast<std::uint64_t>(whole(mark.items[0]));
        entry.crc = static_cast<std::uint32_t>(std::stoul(mark.items[1].text, nullptr, 16));
        out.push_back(std::move(entry));
    }
    return out;
}

} // namespace

std::vector<Mark> inputs(const json::Value &recipe) { return marks(recipe, "inputs"); }
std::vector<Mark> outputs(const json::Value &recipe) { return marks(recipe, "outputs"); }

std::vector<std::pair<std::uint16_t, int>> abilities(const json::Value &recipe) {
    std::vector<std::pair<std::uint16_t, int>> out;
    for (const json::Value &pair : member(member(recipe, "check"), "abilities").items) {
        if (pair.items.size() != 2u) fail("the recipe's abilities are not ids and levels");
        out.emplace_back(static_cast<std::uint16_t>(whole(pair.items[0])), static_cast<int>(whole(pair.items[1])));
    }
    return out;
}

std::uint32_t crc32(std::span<const std::uint8_t> bytes) {
    static const auto table = [] {
        std::array<std::uint32_t, 256> made{};
        for (std::uint32_t i = 0u; i < 256u; ++i) {
            std::uint32_t value = i;
            for (int k = 0; k < 8; ++k) value = (value & 1u) != 0u ? 0xEDB88320u ^ (value >> 1u) : value >> 1u;
            made[i] = value;
        }
        return made;
    }();
    std::uint32_t crc = 0xFFFFFFFFu;
    for (const std::uint8_t byte : bytes) crc = table[(crc ^ byte) & 0xFFu] ^ (crc >> 8u);
    return crc ^ 0xFFFFFFFFu;
}

Built build(const json::Value &recipe_json, const Reader &read) {
    try {
        const Recipe recipe(recipe_json);
        Built built;
        Disc disc{read, {}, {}, {}, {}};
        const std::string objx_path = "obj/p_" + std::string(kCode) + ".objx";
        Fighter base(Span(disc.file(objx_path)));
        std::vector<File> models;
        std::size_t motions = 0u;
        for (const std::string_view model : kModels) {
            File file{"obj/P_" + std::string(kCodeUpper) + "_" + std::string(model) + ".gmo", {}};
            file.bytes = disc.file(file.path);
            const std::size_t count = gmo_model(file.bytes).motions.size();
            if (!models.empty() && count != motions) fail("the models have different motion counts");
            motions = count;
            models.push_back(std::move(file));
        }
        disc.model = models[0].bytes;

        Merge merge(base, recipe, disc, built.notes);
        const std::vector<Bytes> chunks = merge.run(motions);

        std::vector<File> texts, changed;
        const auto take = [&](const std::string &path) {
            if (auto bytes = read(path)) texts.push_back({path, std::move(*bytes)});
        };
        take(std::string(kMainBin));
        for (const std::string_view language : kLanguages) {
            take(names_archive(language));
            take(help_pack(language));
        }
        changed = texts;
        built.abilities = add_abilities(changed, recipe, built.notes);

        built.files.push_back({objx_path, base.to_bytes()});
        for (const File &model : models) built.files.push_back({model.path, gmo_append_motions(model.bytes, chunks)});
        for (std::size_t i = 0u; i < texts.size(); ++i)
            if (changed[i].bytes != texts[i].bytes) built.files.push_back(std::move(changed[i]));
        return built;
    } catch (const effect_package::Error &error) {
        throw Error(std::string("effects: ") + error.what());
    } catch (const gmo_motion::Error &error) {
        throw Error(std::string("animations: ") + error.what());
    }
}

bool check(const json::Value &recipe, const Built &built, std::string &problem) {
    const std::vector<Mark> wanted = outputs(recipe);
    if (wanted.size() != built.files.size()) {
        problem = "the build made " + std::to_string(built.files.size()) + " files, not " + std::to_string(wanted.size());
        return false;
    }
    for (const Mark &mark : wanted) {
        const auto file = std::find_if(built.files.begin(), built.files.end(), [&](const File &f) { return f.path == mark.path; });
        if (file == built.files.end()) {
            problem = mark.path + " was not made";
            return false;
        }
        if (file->bytes.size() != mark.size || crc32(file->bytes) != mark.crc) {
            problem = mark.path + " came to other bytes than the recipe says";
            return false;
        }
    }
    if (built.abilities != abilities(recipe)) {
        problem = "the abilities are not the recipe's";
        return false;
    }
    return true;
}

} // namespace d012::original_moves
