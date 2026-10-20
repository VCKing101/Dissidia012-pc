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
//       is an EX Burst.
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

void fill_ex_gauge(Runtime &rt) {
    const std::uint32_t manager = rt.memory().load32(kBattleManager);
    if (!user_ram(manager)) return;
    const std::uint32_t fighter = rt.memory().load32(manager + kPlayerFighter);
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
    if (!unlock && !ex_full) return;
    // sceDisplayWaitVblankStartCB and sceDisplayWaitVblankStart: once a
    // vblank wait, between two of the game's frames.
    for (const std::uint32_t nid : {0x46F186C3u, 0x984C27E7u}) {
        const portablekit::HleFunction previous = hle.bound_function("sceDisplay", nid);
        if (!previous) continue;
        hle.bind("sceDisplay", nid, [previous, unlock, ex_full](Runtime &rt, AllegrexContext &ctx) {
            if (unlock) unlock_fighters(rt);
            if (ex_full) fill_ex_gauge(rt);
            previous(rt, ctx);
        });
    }
}

} // namespace d012::test_switches
