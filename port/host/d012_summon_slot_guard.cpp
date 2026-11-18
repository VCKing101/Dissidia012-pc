// Empty summonstone slots stay empty. MIT licence; see port/README.md.
//
// The save's inventory (section 1 + 0xE788) keeps 64 summonstone slots of four
// bytes at +0x1484: the summon's id, its uses left, the battles left until it
// recharges, and flags (bit 1: owned). A new game fills the first ones with
// the game's summons (main 0x088B8634, their number at +0x15FC) and leaves the
// rest empty: id -1, no uses, flags 1 (main 0x088B8538).
//
// At the end of a battle, battle 0x08B13098 (section 1 in $a1, the summon used
// in $a3, -1 for none) goes through all 64 slots. For each owned slot it looks
// the summon up (main 0x088C6CA4) and, for the summon that was used, takes a
// use; at none left it reads the summon's recharge from what the lookup gave.
// An empty slot marked as owned has no summon to find, and its id, -1, is also
// what "no summon used" passes: every battle without a summon takes one of its
// uses, and the last one reads address 9, which stops the game before the
// autosave, so the save then stops at the end of every battle.
//
// The All summons cheat used to mark slots 61 to 63 that way, and saved games
// keep them (docs/port/cheats.md). The public cheat list's code still does. So
// this empties such a slot before the game's own run, in the state a new game
// leaves it in. In a sound save it writes nothing. D012_SUMMON_SLOT_GUARD=0
// turns it off.
//
// 0x08B13098 is called through the dispatcher from battle 0x08B08C14, so this
// override takes every call.
#include "d012_decomp_harness.hpp"
#include "d012_summon_slot_rules.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace d012 {
namespace {

namespace decomp = d012::decomp;

constexpr std::uint32_t kUseSummons = 0x08B13098u;  // battle
constexpr std::uint32_t kInventory = 0xE788u;       // in section 1
constexpr std::uint32_t kSlots = 0x1484u;           // in the inventory

bool enabled() {
    const char *text = std::getenv("D012_SUMMON_SLOT_GUARD");
    return text == nullptr || std::strcmp(text, "0") != 0;
}

bool ram(std::uint32_t a) { return a >= 0x08800000u && a < 0x09FF0000u; }

void empty_marked_slots(Guest *g, std::uint32_t section) {
    const std::uint32_t slots = section + kInventory + kSlots;
    if (!ram(slots) || !ram(slots + summon_slots::kSlotCount * 4u - 1u)) return;
    unsigned count = 0;
    for (std::uint32_t i = 0; i < summon_slots::kSlotCount; ++i) {
        const std::uint32_t at = slots + i * 4u;
        const summon_slots::Slot slot{g_ld8(g, at), g_ld8(g, at + 1u), g_ld8(g, at + 2u), g_ld8(g, at + 3u)};
        if (!summon_slots::marked_but_empty(slot)) continue;
        const summon_slots::Slot empty = summon_slots::emptied(slot);
        g_st8(g, at + 1u, empty.uses);
        g_st8(g, at + 2u, empty.recharge);
        g_st8(g, at + 3u, empty.flags);
        ++count;
    }
    if (count != 0u) {
        std::printf("[summons] %u empty summonstone slot(s) were marked as owned: emptied\n", count);
        std::fflush(stdout);
    }
}

void run_use_summons(Guest *g, const decomp::CallState &in, GuestRegs &) {
    if (enabled()) empty_marked_slots(g, in.gpr[5]);
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    decomp::call_original(g, kUseSummons, regs);
}

struct Install {
    Install() {
        static const decomp::Override overrides[] = {
            {kUseSummons, "summon_slot_guard", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
             &run_use_summons},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
    }
} install_overrides;

} // namespace
} // namespace d012
