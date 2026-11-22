// Abilities a mod's package adds, given by level; see d012_mod_abilities.hpp.
// MIT licence; see port/README.md.
//
// Facts (docs/research/roster-and-save.md): a character record is 0x778
// bytes, its character id the s8 at +0, its level the u8 at +1, and its
// learned list 150 entries of {s16 id, s16 AP, u16 flags} from +0x3EE, ended
// by an id of -1 (main 0x088B8414 searches it). main 0x088B1D9C finds an
// ability in the game's table by id (0 when there is none). The accessor
// main 0x088CB164(base, character id) returns the record; it has about 60
// callers across the modules, nearly all in other generated units, so this
// override sees the records the game uses.
#include "d012_mod_abilities.hpp"

#include "d012_decomp_harness.hpp"

#include "hle/hle_common.hpp"
#include "install/user_data.hpp"
#include "portablekit/hle_extension.hpp"
#include "psprecomp/runtime.hpp"

#include <algorithm>
#include <cstdio>
#include <iterator>
#include <filesystem>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <map>
#include <set>
#include <vector>

namespace d012::mod_abilities {
namespace {

namespace decomp = d012::decomp;

constexpr std::uint32_t kRecordOf = 0x088CB164u;
constexpr std::uint32_t kFindAbility = 0x088B1D9Cu;
constexpr std::uint32_t kLevel = 0x001u;
constexpr std::uint32_t kLearned = 0x3EEu;
constexpr std::uint32_t kLearnedCount = 150u;
constexpr std::uint32_t kEntry = 6u;

bool enabled() {
    const char *text = std::getenv("D012_NO_MOD_ABILITIES");
    return text == nullptr || std::strcmp(text, "1") != 0;
}

bool ram(std::uint32_t a) { return a >= 0x08800000u && a < 0x0C000000u; }  // 64 MiB of guest RAM

struct State {
    std::map<int, std::vector<Grant>> by_character;
    std::set<std::uint16_t> missing;  // not in the game's table: never given
    std::set<std::pair<std::uint32_t, std::uint16_t>> logged;
};

State &state() {
    static State s;
    return s;
}

void give(Guest *g, std::uint32_t record, const std::vector<Grant> &grants) {
    State &s = state();
    const int level = g_ld8(g, record + kLevel);
    std::set<std::uint16_t> have;
    std::uint32_t end = kLearnedCount;
    for (std::uint32_t i = 0; i < kLearnedCount; ++i) {
        const std::int16_t id = g_ld16s(g, record + kLearned + kEntry * i);
        if (id == -1) {
            end = i;
            break;
        }
        have.insert(static_cast<std::uint16_t>(id));
    }
    for (const Grant &grant : grants) {
        if (grant.learn <= 0 || grant.learn > level || have.count(grant.id) || s.missing.count(grant.id)) continue;
        if (g_call1(g, kFindAbility, grant.id) == 0u) {
            s.missing.insert(grant.id);
            std::printf("[abilities] %s: ability %04X is not in the game's table; it is not given\n",
                        grant.mod_id.c_str(), grant.id);
            continue;
        }
        if (end >= kLearnedCount) {
            std::printf("[abilities] character %d's learned list is full (%u); %04X is not given\n", grant.id >> 8,
                        kLearnedCount, grant.id);
            return;
        }
        const std::uint32_t at = record + kLearned + kEntry * end;
        g_st16(g, at, grant.id);
        g_st16(g, at + 2u, 0u);
        g_st16(g, at + 4u, 0u);
        ++end;
        if (end < kLearnedCount) g_st16(g, record + kLearned + kEntry * end, 0xFFFFu);
        have.insert(grant.id);
        if (s.logged.insert({record, grant.id}).second)
            std::printf("[abilities] record %08X (character %d, Lv %d): gave %04X (%s)\n", record, grant.id >> 8, level,
                        grant.id, grant.mod_id.c_str());
    }
    std::fflush(stdout);
}

void run_record_of(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    decomp::call_original(g, kRecordOf, regs);
    out.v0 = regs.v0;
    const State &s = state();
    if (s.by_character.empty()) return;
    const std::uint32_t record = regs.v0;
    if (!ram(record) || !ram(record + kLearned + kEntry * kLearnedCount)) return;
    const auto found = s.by_character.find(g_ld8s(g, record));
    if (found != s.by_character.end()) give(g, record, found->second);
}

struct Install {
    Install() {
        static const decomp::Override overrides[] = {
            {kRecordOf, "character_record", decomp::Module::Main, decomp::Returns::Integer, &run_record_of},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], enabled());
    }
} install_overrides;

// --- Saves -------------------------------------------------------------------
//
// The game's data file (DISSIDIA.BIN, 0x3ECC8 bytes) is the save object from
// +0xF08; its 31 character records start at +0x50 (docs/research/
// roster-and-save.md). A record's 5 equipment sets (0xC6 bytes each, from
// +0x10) hold the equipped attacks as one byte per slot, the ability's key
// (its id is the set's character byte at +0xB6 << 8 | key; 0xFF = empty), in
// rows of 30 bytes, one row per form, before the accessories at +0xA2 (main
// 0x088CBDFC walks them through 0x088B7A88 and 0x088B7AB8).
//
// The abilities a mod adds must not reach the save itself: the game without
// the mod (PPSSPP, a PSP, the port with the mod off) would find ids its table
// lacks. So right before PortableKit writes the data (sceUtilitySavedata
// InitStart for a save without a list, the poll that writes for one with a
// list), they are taken out of it: from the learned lists, and from the slots.
// The write sees a save the retail game reads; right after it, they are put
// back. What was taken goes in a side file of the port's own,
// ms0/PSP/MODS/abilities/<game><save>_<file>.txt, read again after a load, so
// their AP, mastery and slots come back. The game's thread is inside the
// utility call the whole time.

constexpr std::uint32_t kDataFileSize = 0x3ECC8u;
constexpr std::uint32_t kRecords = 0x50u;
constexpr std::uint32_t kRecordSize = 0x778u;
constexpr std::uint32_t kRecordCount = 31u;
constexpr std::uint32_t kSets = 0x10u;
constexpr std::uint32_t kSetSize = 0xC6u;
constexpr std::uint32_t kSetCount = 5u;
constexpr std::uint32_t kSetCharacter = 0xB6u;
constexpr std::uint32_t kRowBytes = 30u;  // 12 single slots, then 6 groups of 3
// Rows a character's sets use: one per form (Cecil's Dark Knight and Paladin,
// Lightning's three paradigms), else one; the bytes after them are other
// fields (Bartz's sets hold 0x24 at +0x5F).
std::uint32_t rows_of(int character) { return character == 7 ? 2u : character == 31 ? 3u : 1u; }
// sceUtilitySavedataParam (PortableKit host/hle/hle_savedata.cpp).
constexpr std::uint32_t kResult = 0x1Cu;
constexpr std::uint32_t kMode = 0x30u;
constexpr std::uint32_t kGameName = 0x3Cu;
constexpr std::uint32_t kSaveName = 0x4Cu;
constexpr std::uint32_t kFileName = 0x64u;
constexpr std::uint32_t kDataBuf = 0x74u;
constexpr std::uint32_t kDataSize = 0x7Cu;
constexpr std::uint32_t kStatusQuit = 3u;  // the request is done; the game shuts the utility down next
constexpr std::uint32_t kInitStart = 0x50C4CD57u, kUpdate = 0xD4B95FFBu, kGetStatus = 0x8874DBE0u;

bool save_mode(std::uint32_t m) { return m == 1u || m == 3u || m == 5u || m == 13u || m == 14u || m == 17u || m == 18u; }
bool load_mode(std::uint32_t m) { return m == 0u || m == 2u || m == 4u || m == 15u || m == 16u; }

std::string cstring(psprecomp::GuestMemory &m, std::uint32_t at, std::uint32_t size) {
    std::string out;
    for (std::uint32_t i = 0; i < size; ++i) {
        const char c = static_cast<char>(m.load8(at + i));
        if (c == '\0') break;
        out += c;
    }
    return out;
}

std::uint64_t fnv1a(const std::vector<std::uint8_t> &bytes) {
    std::uint64_t h = 0xCBF29CE484222325ull;
    for (const std::uint8_t b : bytes) h = (h ^ b) * 0x100000001B3ull;
    return h;
}

// What was taken out of a save: learned entries and slot bytes, by record.
struct Taken {
    struct Learned {
        std::uint32_t record;
        std::uint16_t id, ap, flags;
    };
    struct Slot {
        std::uint32_t record, offset;
        std::uint8_t key;
    };
    std::vector<Learned> learned;
    std::vector<Slot> slots;
};

bool granted(std::uint16_t id) {
    const auto found = state().by_character.find(id >> 8);
    if (found == state().by_character.end()) return false;
    for (const Grant &g : found->second)
        if (g.id == id) return true;
    return false;
}

// Takes the mods' abilities out of the data at `buffer`; returns what it took.
Taken take_out(psprecomp::GuestMemory &m, std::uint32_t buffer) {
    Taken taken;
    for (std::uint32_t r = 0; r < kRecordCount; ++r) {
        const std::uint32_t record = buffer + kRecords + kRecordSize * r;
        const std::int8_t character = static_cast<std::int8_t>(m.load8(record));
        if (!state().by_character.count(character)) continue;
        std::uint32_t keep = 0;
        for (std::uint32_t i = 0; i < kLearnedCount; ++i) {
            const std::uint32_t at = record + kLearned + kEntry * i;
            const std::uint16_t id = m.load16(at);
            if (id == 0xFFFFu) break;
            if (granted(id)) {
                taken.learned.push_back({r, id, m.load16(at + 2u), m.load16(at + 4u)});
                continue;
            }
            if (keep != i) {
                const std::uint32_t to = record + kLearned + kEntry * keep;
                for (std::uint32_t k = 0; k < kEntry; k += 2u) m.store16(to + k, m.load16(at + k));
            }
            ++keep;
        }
        if (keep < kLearnedCount) m.store16(record + kLearned + kEntry * keep, 0xFFFFu);
        for (std::uint32_t s = 0; s < kSetCount; ++s) {
            const std::uint32_t set = record + kSets + kSetSize * s;
            const std::uint32_t owner = m.load8(set + kSetCharacter);
            for (std::uint32_t b = 0; b < kRowBytes * rows_of(character); ++b) {
                const std::uint8_t key = m.load8(set + b);
                if (key == 0xFFu || !granted(static_cast<std::uint16_t>(owner << 8u | key))) continue;
                taken.slots.push_back({r, set + b - record, key});
                m.store8(set + b, 0xFFu);
            }
        }
    }
    return taken;
}

std::filesystem::path side_file(psprecomp::GuestMemory &m, std::uint32_t params) {
    // The memory stick as d012_dlc.cpp finds it.
    std::filesystem::path ms0;
    if (const char *dir = portablekit::hle_extension::env("MEMORY_STICK_DIR"); dir != nullptr && *dir != '\0') {
        ms0 = portablekit::install::path_from_utf8(dir);
    } else {
        try {
            ms0 = portablekit::install::user_data_directory() / "ms0";
        } catch (const std::exception &) {
            return {};
        }
    }
    const std::string name = cstring(m, params + kGameName, 13u) + cstring(m, params + kSaveName, 20u) + "_" +
                             cstring(m, params + kFileName, 13u) + ".txt";
    return ms0 / "PSP" / "MODS" / "abilities" / name;
}

void write_side_file(const std::filesystem::path &path, const Taken &taken, std::uint64_t hash) {
    std::error_code error;
    std::filesystem::create_directories(path.parent_path(), error);
    const std::filesystem::path temporary = path.string() + ".tmp";
    if (FILE *f = std::fopen(temporary.string().c_str(), "wb")) {
        std::fprintf(f, "D012 mod abilities 1\nhash %016llX\n", static_cast<unsigned long long>(hash));
        for (const auto &l : taken.learned) std::fprintf(f, "learned %u %04X %u %u\n", l.record, l.id, l.ap, l.flags);
        for (const auto &s : taken.slots) std::fprintf(f, "slot %u %u %02X\n", s.record, s.offset, s.key);
        std::fclose(f);
        std::filesystem::rename(temporary, path, error);
    }
    std::printf("[abilities] the save is written without the mods' abilities; %zu learned and %zu equipped kept in %s\n",
                taken.learned.size(), taken.slots.size(), path.filename().string().c_str());
}

// After a load: the side file's abilities back into the loaded data. The
// slots only when the data is the one written with the file.
void put_back(psprecomp::GuestMemory &m, std::uint32_t params, std::uint32_t buffer) {
    const std::filesystem::path path = side_file(m, params);
    FILE *f = path.empty() ? nullptr : std::fopen(path.string().c_str(), "rb");
    if (f == nullptr) return;
    std::vector<std::uint8_t> data(kDataFileSize);
    m.copy_out(buffer, data);
    const std::uint64_t hash = fnv1a(data);
    char line[128];
    unsigned long long stored = 0;
    std::size_t learned = 0, slots = 0;
    bool same = false;
    while (std::fgets(line, sizeof line, f) != nullptr) {
        unsigned r = 0, id = 0, ap = 0, flags = 0, offset = 0, key = 0;
        if (std::sscanf(line, "hash %llX", &stored) == 1) same = stored == hash;
        else if (std::sscanf(line, "learned %u %X %u %u", &r, &id, &ap, &flags) == 4 && r < kRecordCount && granted(static_cast<std::uint16_t>(id))) {
            const std::uint32_t record = buffer + kRecords + kRecordSize * r;
            std::uint32_t i = 0;
            while (i < kLearnedCount && m.load16(record + kLearned + kEntry * i) != 0xFFFFu &&
                   m.load16(record + kLearned + kEntry * i) != id)
                ++i;
            if (i >= kLearnedCount) continue;
            const bool added = m.load16(record + kLearned + kEntry * i) == 0xFFFFu;
            const std::uint32_t at = record + kLearned + kEntry * i;
            m.store16(at, static_cast<std::uint16_t>(id));
            m.store16(at + 2u, static_cast<std::uint16_t>(ap));
            m.store16(at + 4u, static_cast<std::uint16_t>(flags));
            if (added && i + 1u < kLearnedCount) m.store16(at + kEntry, 0xFFFFu);
            ++learned;
        } else if (std::sscanf(line, "slot %u %u %X", &r, &offset, &key) == 3 && same && r < kRecordCount &&
                   offset >= kSets && offset < kSets + kSetSize * kSetCount) {
            const std::uint32_t at = buffer + kRecords + kRecordSize * r + offset;
            // Only an ability given this start: the set's owner and the key make its id.
            const std::uint32_t set = buffer + kRecords + kRecordSize * r + kSets + (offset - kSets) / kSetSize * kSetSize;
            const std::uint16_t id = static_cast<std::uint16_t>(m.load8(set + kSetCharacter) << 8u | (key & 0xFFu));
            if (m.load8(at) == 0xFFu && granted(id)) {
                m.store8(at, static_cast<std::uint8_t>(key));
                ++slots;
            }
        }
    }
    std::fclose(f);
    std::printf("[abilities] loaded with %s: %zu learned back%s\n", path.filename().string().c_str(), learned,
                same ? (", " + std::to_string(slots) + " equipped back").c_str()
                     : " (the save changed since, so the equipped ones are not)");
    std::fflush(stdout);
}

struct Pending {
    std::uint32_t params{};
    std::uint32_t mode{};
    bool active{};
    Taken taken;           // what the last write left out
    std::uint64_t hash{};  // of the data as written
};

Pending &pending() {
    static Pending p;
    return p;
}

bool ours(psprecomp::GuestMemory &m, std::uint32_t params) {
    return params != 0u && m.load32(params + kDataSize) == kDataFileSize && m.load32(params + kDataBuf) != 0u;
}

// Runs `call` with the mods' abilities out of the data, then puts the data
// back as it was, keeping what was left out for the side file.
void around_write(psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx, const portablekit::HleFunction &call) {
    auto &m = rt.memory();
    const std::uint32_t params = pending().params;
    const std::uint32_t buffer = m.load32(params + kDataBuf);
    std::vector<std::uint8_t> before(kRecordSize * kRecordCount);
    m.copy_out(buffer + kRecords, before);
    const Taken taken = take_out(m, buffer);
    std::vector<std::uint8_t> written(kDataFileSize);
    m.copy_out(buffer, written);
    call(rt, ctx);
    m.copy_in(buffer + kRecords, before);
    pending().taken = taken;
    pending().hash = fnv1a(written);
}

// GetStatus reporting the request done (PortableKit's 3, "quit") with no
// error: a save made, or a load in the buffer before the game reads it.
void finish(psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
    auto &m = rt.memory();
    Pending &p = pending();
    if (ctx.gpr[2] != kStatusQuit) return;
    p.active = false;
    if (m.load32(p.params + kResult) != 0u) return;
    if (save_mode(p.mode)) {
        // With every ability taken back this start (withdraw), nothing could
        // be taken out: the side file of a start that had them stays, so
        // their AP and mastery come back when they do.
        if (state().by_character.empty())
            std::printf("[abilities] the save is written; no added abilities this start, the side file is kept\n");
        else if (const auto path = side_file(m, p.params); !path.empty())
            write_side_file(path, p.taken, p.hash);
    } else {
        put_back(m, p.params, m.load32(p.params + kDataBuf));
    }
}

} // namespace

void install(const std::vector<Grant> &grants) {
    State &s = state();
    for (const Grant &grant : grants) s.by_character[grant.id >> 8].push_back(grant);
    if (!grants.empty())
        std::printf("[abilities] %zu added ability(ies), given by level%s\n", grants.size(),
                    enabled() ? "" : " (off: D012_NO_MOD_ABILITIES)");
}

void withdraw(const std::string &mod_id) {
    State &s = state();
    std::size_t taken = 0;
    for (auto it = s.by_character.begin(); it != s.by_character.end();) {
        auto &list = it->second;
        const std::size_t before = list.size();
        std::erase_if(list, [&](const Grant &g) { return g.mod_id == mod_id; });
        taken += before - list.size();
        it = list.empty() ? s.by_character.erase(it) : std::next(it);
    }
    if (taken != 0u) std::printf("[abilities] %zu ability(ies) of %s taken back\n", taken, mod_id.c_str());
}

void install_saves(portablekit::HleRegistrar &hle) {
    if (state().by_character.empty() || !enabled()) return;
    const portablekit::HleFunction init = hle.bound_function("sceUtility", kInitStart);
    const portablekit::HleFunction update = hle.bound_function("sceUtility", kUpdate);
    const portablekit::HleFunction status = hle.bound_function("sceUtility", kGetStatus);
    if (!init || !update || !status) {
        std::cerr << "[abilities] the save utility is not bound; the mods' abilities would reach the save, so they are off\n";
        state().by_character.clear();
        return;
    }
    hle.bind("sceUtility", kInitStart, [init](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
        auto &m = rt.memory();
        const std::uint32_t params = ctx.gpr[4];
        Pending &p = pending();
        p = {};
        if (!ours(m, params)) {
            init(rt, ctx);
            return;
        }
        p.params = params;
        p.mode = m.load32(params + kMode);
        p.active = save_mode(p.mode) || load_mode(p.mode);
        if (save_mode(p.mode)) around_write(rt, ctx, init);
        else init(rt, ctx);
    });
    for (const auto &[nid, original] : {std::pair{kUpdate, update}, std::pair{kGetStatus, status}}) {
        const bool status_call = nid == kGetStatus;
        hle.bind("sceUtility", nid, [original, status_call](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
            const Pending &p = pending();
            if (!p.active) {
                original(rt, ctx);
                return;
            }
            if (save_mode(p.mode)) around_write(rt, ctx, original);
            else original(rt, ctx);
            if (status_call) finish(rt, ctx);
        });
    }
}

} // namespace d012::mod_abilities
