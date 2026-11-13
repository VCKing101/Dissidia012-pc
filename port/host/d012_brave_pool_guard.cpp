// The Brave numbers' pool, kept whole (issue 16). MIT licence; see
// port/README.md.
//
// The battle's Brave display (battle 0x089DDFC8 builds it) shows the Brave
// numbers that fly off the fighters. It keeps 14 entries (0x54 bytes each,
// at +0x190) and a ring of 14 slots {entry, next} at +0x120 that holds the
// free ones: a slot is empty (0) or holds one of the 14. The take and give
// cursors are at +0x628 and +0x62C, the entries in use in a list from +0x630
// (head) and +0x634 (tail), linked through each entry's +0 (next) and +4
// (previous), and their count at +0x638. Each frame the display's update
// (battle 0x089DE360, with the display in $a0) turns the frame's Brave
// events into entries taken from the ring; the take resets the entry it
// takes (battle 0x089E82C8) and skips an empty slot.
//
// Issue 16 crashed there: a slot held 0x00004E20, which is not an entry, and
// the reset wrote to guest address 0x4E28. That is the PP cheat's 20,000: in
// that battle slot 12 lies at 0x096B2DC0, the PP Catalog's copy of PP in the
// menus, which the cheat wrote at all times (issue 20 is the same write
// meeting Yuna's animations). The port's own cheat now writes the save's PP
// (d012_cheats.cpp), but a cheat file can still carry the old code, and
// PPSSPP lets such a write go by. So this puts the ring right before each
// update: a slot that holds anything but 0 or an entry gets an entry that is
// neither in the ring nor in use, or 0, so the take never resets something
// that is not an entry. In a sound ring it writes nothing. Each repair is
// logged, at most four for a display.
//
// The update is called from battle 0x089E7D60, in another generated unit, so
// this override takes every call. D012_BRAVE_POOL_GUARD=0 turns it off.
#include "d012_decomp_harness.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace d012 {
namespace {

namespace decomp = d012::decomp;

constexpr std::uint32_t kUpdate = 0x089DE360u;
constexpr std::uint32_t kSlots = 14u;
constexpr std::uint32_t kRing = 0x120u;
constexpr std::uint32_t kEntries = 0x190u;
constexpr std::uint32_t kEntrySize = 0x54u;
constexpr std::uint32_t kTake = 0x628u;
constexpr std::uint32_t kGive = 0x62Cu;
constexpr std::uint32_t kHead = 0x630u;
constexpr unsigned kLogsPerDisplay = 4u;

bool enabled() {
    const char *text = std::getenv("D012_BRAVE_POOL_GUARD");
    return text == nullptr || std::strcmp(text, "0") != 0;
}

bool ram(std::uint32_t a) { return a >= 0x08800000u && a < 0x09FF0000u; }
std::uint32_t slot(std::uint32_t m, std::uint32_t i) { return m + kRing + 8u * i; }
std::uint32_t entry(std::uint32_t m, std::uint32_t i) { return m + kEntries + kEntrySize * i; }

// The entry's index, or kSlots when `a` is not one of the display's entries.
std::uint32_t entry_index(std::uint32_t m, std::uint32_t a) {
    if (a < entry(m, 0) || a >= entry(m, kSlots) || (a - entry(m, 0)) % kEntrySize != 0u) return kSlots;
    return (a - entry(m, 0)) / kEntrySize;
}

bool is_slot(std::uint32_t m, std::uint32_t a) {
    return a >= slot(m, 0) && a < slot(m, kSlots) && (a - slot(m, 0)) % 8u == 0u;
}

std::uint32_t g_display = 0;
unsigned g_logs = 0;

void report(std::uint32_t m, const char *what, std::uint32_t at, std::uint32_t was, std::uint32_t now) {
    if (g_logs >= kLogsPerDisplay) return;
    ++g_logs;
    std::printf("[brave-pool] the Brave display at %08X: %s at %08X held %08X, now %08X\n", m, what, at, was, now);
    std::fflush(stdout);
}

void repair(Guest *g, std::uint32_t m) {
    if (m != g_display) {
        g_display = m;
        g_logs = 0;
    }
    // Only a ring that is linked as the display links it: anything else is
    // not this structure, and is left alone.
    unsigned linked = 0;
    for (std::uint32_t i = 0; i < kSlots; ++i) {
        linked += g_ld32(g, slot(m, i) + 4u) == slot(m, (i + 1u) % kSlots);
    }
    if (linked < kSlots - 2u) return;

    bool used[kSlots] = {};
    bool bad[kSlots] = {};
    bool any_bad = false;
    for (std::uint32_t i = 0; i < kSlots; ++i) {
        const std::uint32_t value = g_ld32(g, slot(m, i));
        if (value == 0u) continue;
        const std::uint32_t index = entry_index(m, value);
        if (index == kSlots || used[index]) {
            bad[i] = true;
            any_bad = true;
        } else {
            used[index] = true;
        }
    }
    bool cursors_bad = !is_slot(m, g_ld32(g, m + kTake)) || !is_slot(m, g_ld32(g, m + kGive));
    for (std::uint32_t i = 0; i < kSlots; ++i) {
        any_bad = any_bad || g_ld32(g, slot(m, i) + 4u) != slot(m, (i + 1u) % kSlots);
    }
    if (!any_bad && !cursors_bad) return;

    // The entries in use: the list from the head.
    std::uint32_t e = g_ld32(g, m + kHead);
    for (unsigned steps = 0; e != 0u && steps < kSlots; ++steps) {
        const std::uint32_t index = entry_index(m, e);
        if (index == kSlots) break;
        used[index] = true;
        e = g_ld32(g, e);
    }
    std::uint32_t spare = 0;
    for (std::uint32_t i = 0; i < kSlots; ++i) {
        const std::uint32_t next_wanted = slot(m, (i + 1u) % kSlots);
        const std::uint32_t next = g_ld32(g, slot(m, i) + 4u);
        if (next != next_wanted) {
            g_st32(g, slot(m, i) + 4u, next_wanted);
            report(m, "a slot's next", slot(m, i) + 4u, next, next_wanted);
        }
        if (!bad[i]) continue;
        while (spare < kSlots && used[spare]) ++spare;
        const std::uint32_t put = spare < kSlots ? entry(m, spare) : 0u;
        if (spare < kSlots) used[spare] = true;
        const std::uint32_t was = g_ld32(g, slot(m, i));
        g_st32(g, slot(m, i), put);
        report(m, "a slot", slot(m, i), was, put);
    }
    for (std::uint32_t offset : {kTake, kGive}) {
        const std::uint32_t cursor = g_ld32(g, m + offset);
        if (!is_slot(m, cursor)) {
            g_st32(g, m + offset, slot(m, 0));
            report(m, offset == kTake ? "the take cursor" : "the give cursor", m + offset, cursor, slot(m, 0));
        }
    }
}

void run_update(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    const std::uint32_t m = in.gpr[4];
    if (ram(m) && g_ld8(g, m) != 0u) repair(g, m);
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    decomp::call_original(g, kUpdate, regs);
    out.v0 = regs.v0;
}

struct Install {
    Install() {
        static const decomp::Override overrides[] = {
            {kUpdate, "brave_display_update", decomp::Module::BattleOverlay, decomp::Returns::Integer, &run_update},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], enabled());
    }
} install_overrides;

} // namespace
} // namespace d012
