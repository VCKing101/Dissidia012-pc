// Test switches: scenes made reachable on purpose, for checking the renderer
// against the same moment run after run (docs/port/graphics-triage.md). All
// off by default; none changes anything a player would see without them.
// MIT licence; see port/README.md.
//
//   D012_TEST_UNLOCK_FIGHTERS=1  every playable fighter can be picked on the
//       select screens (Quick Battle's player and CPU grids), as after the
//       story: the unlock flags are set in the running game's flag field,
//       so an autosave keeps them. Use it with a copy of the data directory.
//   D012_TEST_EX_FULL=1          in battle, the player's EX gauge is kept full,
//       so R + square enters EX Mode at once and the next HP attack that lands
//       is an EX Burst. =2: the opponent's too, for player 2 in local versus.
//   D012_TEST_LOW_HP=1           once a battle, the player's maximum HP set to
//       50 (status block +0x0C), so the opponent wins at the next HP attack.
//
// Facts (docs/research/roster-and-save.md, private research notes on the
// status block): the save's flag field is at *(main 0x089D726C), tested as
// bit `idx & 31` of word `idx >> 5`; a fighter's unlock flag is 0x2BC plus its
// save slot, the 31 slots in a row. The battle manager is *(0x089D6A10), its
// fighters at +0x38 (player) and +0x3C; a fighter's status block is the
// pointer at +0x52C, with the EX gauge a float from 0 to 10000 at +0x18.

#include "d012_test_switches.hpp"

#include "hle/hle_common.hpp"
#include "profile.hpp"

#include "psprecomp/runtime.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>

namespace d012::test_switches {
namespace {

using psprecomp::AllegrexContext;
using psprecomp::Runtime;

constexpr std::uint32_t kFlagFieldPointer = 0x089D726Cu;
constexpr std::uint32_t kFirstUnlockFlag = 0x2BCu;
constexpr std::uint32_t kFighterSlots = 31u;
constexpr std::uint32_t kBattleManager = 0x089D6A10u;
constexpr std::uint32_t kPlayerFighter = 0x38u;
constexpr std::uint32_t kOpponentFighter = 0x3Cu;
constexpr std::uint32_t kStatusBlock = 0x52Cu;
constexpr std::uint32_t kExGauge = 0x18u;
constexpr float kExFull = 10000.0f;

bool user_ram(std::uint32_t address) {
    const std::uint32_t a = address & 0x1FFFFFFFu;
    return a >= 0x08800000u && a < 0x0A000000u - 4u;
}

bool flag(const char *name) {
    const char *text = portablekit::env(name);
    return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
}

void unlock_fighters(Runtime &rt) {
    const std::uint32_t field = rt.memory().load32(kFlagFieldPointer);
    if (!user_ram(field)) return;
    bool changed = false;
    for (std::uint32_t slot = 0; slot < kFighterSlots; ++slot) {
        const std::uint32_t index = kFirstUnlockFlag + slot;
        const std::uint32_t at = field + (index >> 5u) * 4u;
        const std::uint32_t word = rt.memory().load32(at);
        const std::uint32_t bit = 1u << (index & 31u);
        if ((word & bit) == 0u) {
            rt.memory().store32(at, word | bit);
            changed = true;
        }
    }
    static bool told = false;
    if (changed && !told) {
        told = true;
        std::cout << "[d012] test: every fighter unlocked (D012_TEST_UNLOCK_FIGHTERS)\n";
    }
}

void fill_ex_gauge(Runtime &rt, std::uint32_t slot_offset);

// D012_TEST_LOW_HP=1: once a battle, the player's maximum HP (status +0x0C)
// set to 50, which brings the HP down to it, so that the opponent wins at the
// next HP attack (versus endings). The HP itself is elsewhere (not found).
void lower_hp(Runtime &rt) {
    static std::uint32_t lowered = 0;  // the status block lowered last
    const std::uint32_t manager = rt.memory().load32(kBattleManager);
    if (!user_ram(manager)) return;
    const std::uint32_t fighter = rt.memory().load32(manager + kPlayerFighter);
    if (!user_ram(fighter)) return;
    const std::uint32_t status = rt.memory().load32(fighter + kStatusBlock);
    if (!user_ram(status) || status == lowered) return;
    const std::uint32_t hp = rt.memory().load32(status + 0x0Cu);
    if (hp <= 50u || hp > 99999u) return;
    rt.memory().store32(status + 0x0Cu, 50u);
    lowered = status;
    std::cout << "[d012] test: the player's HP lowered to 50 (D012_TEST_LOW_HP)\n";
}

void fill_ex_gauges(Runtime &rt, bool both) {
    fill_ex_gauge(rt, kPlayerFighter);
    if (both) fill_ex_gauge(rt, kOpponentFighter);
}

void fill_ex_gauge(Runtime &rt, std::uint32_t slot_offset) {
    const std::uint32_t manager = rt.memory().load32(kBattleManager);
    if (!user_ram(manager)) return;
    const std::uint32_t fighter = rt.memory().load32(manager + slot_offset);
    if (!user_ram(fighter)) return;
    const std::uint32_t status = rt.memory().load32(fighter + kStatusBlock);
    if (!user_ram(status)) return;
    const std::uint32_t bits = rt.memory().load32(status + kExGauge);
    float gauge = 0.0f;
    std::memcpy(&gauge, &bits, sizeof(gauge));
    // Only a gauge that reads as one: anything else is not a fighter's status.
    if (!std::isfinite(gauge) || gauge < 0.0f || gauge > kExFull || gauge == kExFull) return;
    std::uint32_t full = 0u;
    std::memcpy(&full, &kExFull, sizeof(full));
    rt.memory().store32(status + kExGauge, full);
    static bool told = false;
    if (!told) {
        told = true;
        std::cout << "[d012] test: the player's EX gauge kept full (D012_TEST_EX_FULL), status block 0x" << std::hex
                  << status << std::dec << "\n";
    }
}

} // namespace

void install(portablekit::HleRegistrar &hle) {
    const bool unlock = flag("TEST_UNLOCK_FIGHTERS");
    const bool ex_full = flag("TEST_EX_FULL");
    // D012_TEST_EX_FULL=2: the opponent's gauge too (local versus).
    const char *ex_text = portablekit::env("TEST_EX_FULL");
    const bool ex_both = ex_full && std::strcmp(ex_text, "2") == 0;
    const bool low_hp = flag("TEST_LOW_HP");
    if (!unlock && !ex_full && !low_hp) return;
    // sceDisplayWaitVblankStartCB and sceDisplayWaitVblankStart: once a
    // vblank wait, between two of the game's frames.
    for (const std::uint32_t nid : {0x46F186C3u, 0x984C27E7u}) {
        const portablekit::HleFunction previous = hle.bound_function("sceDisplay", nid);
        if (!previous) continue;
        hle.bind("sceDisplay", nid, [previous, unlock, ex_full, ex_both, low_hp](Runtime &rt, AllegrexContext &ctx) {
            if (unlock) unlock_fighters(rt);
            if (ex_full) fill_ex_gauges(rt, ex_both);
            if (low_hp) lower_hp(rt);
            previous(rt, ctx);
        });
    }
}

} // namespace d012::test_switches
