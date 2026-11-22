// Hitbox groups placed at battle start, for movesets with more groups than
// a fighter has ids (Bartz with his original Dissidia moves). MIT licence;
// see port/README.md.
//
// A fighter's collision shapes (objx kind 5, 32-byte records) are switched
// in groups: an event names a group by id, and every record whose byte 0 is
// that id goes on or off. The masks are 64 bits wide, and the game makes a
// group's bit with a 64-bit shift (main 0x08837D18 calls 0x089985B0), so an
// id of 64 or more acts as id - 32. tools/original_moves.py stores the
// groups a moveset has no room for "parked": record byte 0 = 63 (a bit no
// fighter uses), byte 1 = the group's number (64..255), and the events of
// the group carry 63 + 256 x number, whose shift lands on bit 63 as well.
//
// The battle works out which commands a fighter can use with battle
// 0x08AB7F40(scratch, combos, record, set): it marks, in a 256-bit mask at
// scratch + 0x24, the entries of the combo table (objx `comb`) that an
// equipped ability enables. After it, this places the parked groups the
// enabled commands use on ids nothing else of the fighter's can use: ids
// with no records, then ids whose groups only commands that are not enabled
// use (their records are parked in turn). It rewrites the records' byte 0
// and the events of the animations that can play, in the fighter's own
// copies of the tables (the object loader copies them per fighter). Every
// change is journalled, so a later call with other commands undoes and
// places again, and data the game has since loaded again is left alone.
//
// Facts: the battle manager is *(0x089D6A10), its fighters at +0x38 and
// +0x3C; a fighter's object data is the pointer at +0x390, with the shapes
// at +0x18, the motion table at +0x1C, the event tracks at +0x20, the attack
// records at +0x88 and the combo table at +0x8C (docs/research/
// moveset-format.md). The marking function is called from other generated
// units, so this override takes every call. D012_NO_SHAPE_IDS=1 turns it off.
#include "d012_decomp_harness.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <set>
#include <vector>

namespace d012 {
namespace {

namespace decomp = d012::decomp;

constexpr std::uint32_t kMarkCommands = 0x08AB7F40u;
constexpr std::uint32_t kBattle = 0x089D6A10u;
constexpr std::uint32_t kFighters = 0x38u;
constexpr std::uint32_t kObjectData = 0x390u;
constexpr std::uint32_t kShapes = 0x18u;
constexpr std::uint32_t kMotions = 0x1Cu;
constexpr std::uint32_t kTracks = 0x20u;
constexpr std::uint32_t kAttacks = 0x88u;
constexpr std::uint32_t kCombos = 0x8Cu;
constexpr std::uint32_t kMarks = 0x24u;
constexpr std::uint8_t kParked = 63u;
constexpr std::uint32_t kShapeRecord = 0x20u;
constexpr std::uint32_t kAttackRecord = 0x74u;

bool enabled() {
    const char *text = std::getenv("D012_NO_SHAPE_IDS");
    return text == nullptr || std::strcmp(text, "1") != 0;
}

// Guest RAM, the 64 MiB the profile gives included: with a model bigger than
// the game's block the file heap and the battle model pool, and with them a
// fighter's tables, sit above 0x0A000000 (d012_model_memory.cpp).
bool ram(std::uint32_t a) { return a >= 0x08800000u && a < 0x0C000000u; }

std::uint16_t ld16(Guest *g, std::uint32_t a) { return static_cast<std::uint16_t>(g_ld16s(g, a)); }

// A word or byte this changed, to put back.
struct Change {
    std::uint32_t at;
    std::uint32_t size;  // 1 or 4
    std::uint32_t was;
    std::uint32_t now;
};

struct Journal {
    std::uint32_t shapes{};
    std::array<std::uint32_t, 8> marks{};
    std::vector<Change> changes;
};

std::map<std::uint32_t, Journal> g_journals;  // by object data

std::uint32_t load(Guest *g, const Change &c) { return c.size == 1u ? g_ld8(g, c.at) : g_ld32(g, c.at); }

void store(Guest *g, Journal &j, std::uint32_t at, std::uint32_t size, std::uint32_t value) {
    const Change c{at, size, size == 1u ? g_ld8(g, at) : g_ld32(g, at), value};
    if (c.was == value) return;
    if (size == 1u) g_st8(g, at, static_cast<std::uint8_t>(value));
    else g_st32(g, at, value);
    j.changes.push_back(c);
}

// The journal still describes memory: everything it wrote is still there.
bool intact(Guest *g, const Journal &j) {
    return std::all_of(j.changes.begin(), j.changes.end(), [&](const Change &c) { return load(g, c) == c.now; });
}

void undo(Guest *g, Journal &j) {
    for (auto c = j.changes.rbegin(); c != j.changes.rend(); ++c) {
        if (c->size == 1u) g_st8(g, c->at, static_cast<std::uint8_t>(c->was));
        else g_st32(g, c->at, c->was);
    }
    j.changes.clear();
}

// An event argument that names a hitbox group, and where it is.
struct ShapeArg {
    std::uint32_t at;
    std::uint32_t value;
};

struct Moveset {
    std::map<std::uint16_t, std::uint16_t> anim_of_motion;
    std::map<std::uint16_t, std::vector<ShapeArg>> shape_args;    // by animation
    std::map<std::uint16_t, std::vector<std::uint16_t>> follow_ups;  // attacks an animation offers
    std::set<std::uint16_t> anims;
};

Moveset read_moveset(Guest *g, std::uint32_t motions, std::uint32_t tracks) {
    Moveset m;
    const std::uint32_t motion_count = g_ld32(g, motions + 4u);
    for (std::uint32_t i = 0; i < motion_count && i < 4096u; ++i) {
        const std::uint32_t e = motions + 8u + 8u * i;
        m.anim_of_motion[ld16(g, e)] = ld16(g, e + 2u);
    }
    const std::uint32_t track_count = g_ld32(g, tracks + 4u);
    for (std::uint32_t i = 0; i < track_count && i < 4096u; ++i) {
        const std::uint32_t t = tracks + g_ld32(g, tracks + 8u + 4u * i);
        const std::uint16_t anim = ld16(g, t);
        const std::uint32_t ranged = g_ld8(g, t + 2u);
        m.anims.insert(anim);
        std::uint32_t cursor = t + 8u;
        for (std::uint32_t k = 0; k < ranged; ++k) {
            const std::uint32_t type = g_ld8(g, cursor + 4u), argc = g_ld8(g, cursor + 5u);
            const std::uint32_t args = cursor + 12u;
            int shape_arg = -1;
            if ((type == 0x00u || type == 0x43u || type == 0x7Bu) && argc > 1u) shape_arg = 1;
            if ((type == 0x12u || type == 0x4Eu) && argc > 0u) shape_arg = 0;
            if (shape_arg >= 0) {
                const std::uint32_t at = args + 4u * static_cast<std::uint32_t>(shape_arg);
                m.shape_args[anim].push_back({at, g_ld32(g, at)});
            }
            if ((type == 0x67u || type == 0x79u) && argc > 1u)
                m.follow_ups[anim].push_back(static_cast<std::uint16_t>(g_ld32(g, args + 4u)));
            cursor = args + 4u * argc;
        }
    }
    return m;
}

// The animations a combo entry can play: its stages' attacks, their motions,
// the attacks they upgrade to and the follow-ups their events offer.
std::set<std::uint16_t> anims_of_entry(Guest *g, const Moveset &m, std::uint32_t entry, std::uint32_t attacks) {
    std::set<std::uint16_t> out;
    std::vector<std::uint16_t> pending;
    const std::int16_t stages = g_ld16s(g, entry + 2u);
    for (std::int16_t s = 0; s < stages && s < 64; ++s) pending.push_back(ld16(g, entry + 8u + 8u * static_cast<std::uint32_t>(s)));
    const std::uint32_t attack_count = g_ld32(g, attacks + 4u);
    std::set<std::uint16_t> seen;
    while (!pending.empty()) {
        const std::uint16_t id = pending.back();
        pending.pop_back();
        if (!seen.insert(id).second) continue;
        for (std::uint32_t i = 0; i < attack_count && i < 4096u; ++i) {
            const std::uint32_t r = attacks + 8u + kAttackRecord * i;
            if (ld16(g, r) != id) continue;
            const std::int16_t upgrade = g_ld16s(g, r + 0x0Au);
            if (upgrade >= 0) pending.push_back(static_cast<std::uint16_t>(upgrade));
            for (const std::uint32_t field : {0x1Cu, 0x10u}) {
                const auto found = m.anim_of_motion.find(ld16(g, r + field));
                if (found == m.anim_of_motion.end()) continue;
                out.insert(found->second);
                const auto offered = m.follow_ups.find(found->second);
                if (offered != m.follow_ups.end())
                    pending.insert(pending.end(), offered->second.begin(), offered->second.end());
            }
            break;
        }
    }
    return out;
}

void place(Guest *g, std::uint32_t data, const std::array<std::uint32_t, 8> &marks, Journal &j) {
    const std::uint32_t shapes = g_ld32(g, data + kShapes), motions = g_ld32(g, data + kMotions);
    const std::uint32_t tracks = g_ld32(g, data + kTracks), attacks = g_ld32(g, data + kAttacks);
    const std::uint32_t combos = g_ld32(g, data + kCombos);
    if (!ram(shapes) || !ram(motions) || !ram(tracks) || !ram(attacks) || !ram(combos)) return;

    // The records by id, and the parked groups.
    const std::uint32_t count = g_ld32(g, shapes + 8u);
    const std::uint64_t default_on = static_cast<std::uint64_t>(g_ld32(g, shapes)) |
                                     static_cast<std::uint64_t>(g_ld32(g, shapes + 4u)) << 32u;
    std::map<std::uint32_t, std::vector<std::uint32_t>> records;  // id -> record addresses
    std::map<std::uint32_t, std::vector<std::uint32_t>> parked;   // group -> record addresses
    for (std::uint32_t i = 0; i < count && i < 4096u; ++i) {
        const std::uint32_t r = shapes + 16u + kShapeRecord * i;
        const std::uint8_t id = g_ld8(g, r), group = g_ld8(g, r + 1u);
        if (id == kParked && group >= 64u) parked[group].push_back(r);
        else records[id].push_back(r);
    }
    if (parked.empty()) return;

    const Moveset m = read_moveset(g, motions, tracks);
    std::set<std::uint16_t> command_anims, can_play;
    const std::int16_t entries = g_ld16s(g, combos + 2u);
    for (std::int16_t e = 0; e < entries && e < 256; ++e) {
        const std::uint32_t entry = combos + g_ld32(g, combos + 4u + 4u * static_cast<std::uint32_t>(e));
        const auto anims = anims_of_entry(g, m, entry, attacks);
        command_anims.insert(anims.begin(), anims.end());
        if (marks[static_cast<std::uint32_t>(e) >> 5u] >> (static_cast<std::uint32_t>(e) & 31u) & 1u)
            can_play.insert(anims.begin(), anims.end());
    }
    for (const std::uint16_t anim : m.anims)
        if (!command_anims.count(anim)) can_play.insert(anim);

    // Which ids the animations that can play switch, and which ids only the
    // others do.
    std::set<std::uint32_t> wanted_groups, used_ids, other_ids;
    for (const auto &[anim, args] : m.shape_args)
        for (const ShapeArg &a : args) {
            const bool plays = can_play.count(anim) != 0u;
            if ((a.value & 0xFFu) == kParked && a.value >= 256u) {
                if (plays) wanted_groups.insert(a.value >> 8u);
            } else if (a.value < 64u) {
                (plays ? used_ids : other_ids).insert(a.value);
            }
        }
    std::vector<std::uint32_t> pool;
    for (std::uint32_t id = 0; id < kParked; ++id)
        if (!records.count(id) && !(default_on >> id & 1u) && !used_ids.count(id) && !other_ids.count(id)) pool.push_back(id);
    for (std::uint32_t id = 0; id < kParked; ++id)
        if (records.count(id) && !(default_on >> id & 1u) && !used_ids.count(id) && other_ids.count(id)) pool.push_back(id);

    std::vector<std::uint32_t> placed, left;
    std::size_t next = 0;
    for (const std::uint32_t group : wanted_groups) {
        if (!parked.count(group)) continue;
        if (next >= pool.size()) {
            left.push_back(group);
            continue;
        }
        const std::uint32_t id = pool[next++];
        for (const std::uint32_t r : records[id]) store(g, j, r, 1u, kParked);  // a group nothing here can play
        for (const std::uint32_t r : parked[group]) store(g, j, r, 1u, id);
        for (const auto &[anim, args] : m.shape_args)
            if (can_play.count(anim))
                for (const ShapeArg &a : args)
                    if (a.value == kParked + 256u * group) store(g, j, a.at, 4u, id);
        placed.push_back(id);
    }
    std::printf("[shape-ids] fighter data %08X: %zu hitbox group(s) of the equipped commands placed", data, placed.size());
    if (!placed.empty()) {
        std::printf(" on id");
        for (const std::uint32_t id : placed) std::printf(" %u", id);
    }
    if (!left.empty()) std::printf("; %zu without a free id, they will not hit", left.size());
    std::printf("\n");
    std::fflush(stdout);
}

// The object data of the battle fighter whose combo table is `combos`.
std::uint32_t fighter_data(Guest *g, std::uint32_t combos) {
    const std::uint32_t battle = g_ld32(g, kBattle);
    if (!ram(battle)) return 0u;
    for (std::uint32_t side = 0; side < 2u; ++side) {
        const std::uint32_t fighter = g_ld32(g, battle + kFighters + 4u * side);
        if (!ram(fighter)) continue;
        const std::uint32_t data = g_ld32(g, fighter + kObjectData);
        if (ram(data) && g_ld32(g, data + kCombos) == combos) return data;
    }
    return 0u;
}

void run_mark(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    decomp::call_original(g, kMarkCommands, regs);
    out.v0 = regs.v0;

    const std::uint32_t scratch = in.gpr[4], combos = in.gpr[5];
    if (!ram(scratch) || !ram(combos)) return;
    const std::uint32_t data = fighter_data(g, combos);
    if (data == 0u) return;
    std::array<std::uint32_t, 8> marks{};
    for (std::uint32_t i = 0; i < 8u; ++i) marks[i] = g_ld32(g, scratch + kMarks + 4u * i);
    // The game asks for more than the fighter's loadout (one HP command at the
    // start, each of Lightning's forms in turn): the groups go in for every
    // command enabled so far, and are placed again only when that grows.
    Journal &j = g_journals[data];
    const std::uint32_t shapes = g_ld32(g, data + kShapes);
    if (j.shapes == shapes && intact(g, j)) {
        std::array<std::uint32_t, 8> all = j.marks;
        for (std::uint32_t i = 0; i < 8u; ++i) all[i] |= marks[i];
        if (all == j.marks) return;
        marks = all;
        undo(g, j);
    } else {
        j.changes.clear();
    }
    j.shapes = shapes;
    j.marks = marks;
    place(g, data, marks, j);
}

struct Install {
    Install() {
        static const decomp::Override overrides[] = {
            {kMarkCommands, "mark_equipped_commands", decomp::Module::BattleOverlay, decomp::Returns::Integer, &run_mark},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], enabled());
    }
} install_overrides;

} // namespace
} // namespace d012