// The AP cheat (the Cheats page's AP row, d012_cheats.cpp): a battle that
// gives AP gives the amount chosen. MIT licence; see port/README.md.
//
// At the end of a battle (battle 0x08B07324) the game works out each side's
// rewards for each fight (battle 0x08B0AD88) into a record at
// FIELD + side * 0x1BD8 + fight * 0x948 + 0xD8, through battle 0x08ADDCF8
// with the record in $a1 and the fighter in $a2. When the battle gives AP
// ($t2 = 1), battle 0x08ADDFE4 puts it at +0x170 (battle 0x08ADE14C: the
// reward row's AP and bonuses, times the accessories' AP bonus), then the
// abilities that AP masters (main 0x088B9998: up to 150 at +0, their count
// at +0x12C) and the abilities mastering them teaches (main 0x088B9BA0: up
// to 32 at +0x12E, their count at +0x16E). The results list (battle
// 0x08B0B71C, in the same call of 0x08B07324) shows the fights' AP added up.
// Later, battle 0x08B08C14 (from 0x08B07974) adds each fight's AP to the
// abilities the fighter has on (main 0x088B8038, each ability up to the AP
// that masters it: main 0x088B7F58).
//
// The public list's AP codes write 0x09AEA568, side 0's first record's
// +0x170. The game writes it and lists it in one call, so a cheat run every
// 77 ms cannot change what the results show.
//
// This override runs the game's own; then, for the player's fighter in a
// battle that gave AP, it raises +0x170 to the amount chosen and works out
// the two lists again as battle 0x08ADDFE4 does. 0x08ADDCF8 is called from
// battle 0x08B0AD88, in another generated unit, so this takes every call.
#include "d012_cheats.hpp"
#include "d012_decomp_harness.hpp"

#include <cstdint>
#include <iostream>

namespace d012 {
namespace {

namespace decomp = d012::decomp;

constexpr std::uint32_t kWorkOutRewards = 0x08ADDCF8u;  // battle
constexpr std::uint32_t kMastered = 0x088B9998u;        // main
constexpr std::uint32_t kTaught = 0x088B9BA0u;          // main
constexpr std::uint32_t kField = 0x089D6A10u;           // the battle's state
constexpr std::uint32_t kFieldFighters = 0x38u;         // a fighter per side
constexpr std::uint32_t kFieldPlayerSide = 0x54u;
constexpr std::uint32_t kFighterRecord = 0x30Cu;        // the character's record in the battle
constexpr std::uint32_t kAp = 0x170u;
constexpr std::uint32_t kMasteredCount = 0x12Cu;
constexpr std::uint32_t kTaughtList = 0x12Eu;
constexpr std::uint32_t kTaughtCount = 0x16Eu;

bool ram(std::uint32_t a) { return a >= 0x08800000u && a < 0x0A000000u; }

bool player_fighter(Guest *g, std::uint32_t fighter) {
    const std::uint32_t field = g_ld32(g, kField);
    if (!ram(field)) return false;
    const std::uint32_t side = g_ld32(g, field + kFieldPlayerSide);
    return side < 2u && fighter != 0u && g_ld32(g, field + kFieldFighters + 4u * side) == fighter;
}

void run_work_out_rewards(Guest *g, const decomp::CallState &in, GuestRegs &) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    decomp::call_original(g, kWorkOutRewards, regs);

    const std::uint32_t record = in.gpr[5];
    const std::uint32_t fighter = in.gpr[6];
    if ((in.gpr[10] & 0xFFu) != 1u || !ram(record)) return;
    const std::uint32_t amount = ap_after_battle();
    if (amount == 0u) return;
    const std::int32_t given = g_ld32s(g, record + kAp);
    if (given <= 0 || static_cast<std::uint32_t>(given) >= amount || !player_fighter(g, fighter)) return;
    const std::uint32_t character = g_ld32(g, fighter + kFighterRecord);
    if (!ram(character)) return;

    g_st32(g, record + kAp, amount);
    const std::uint32_t sets = character + 0x10u;
    const std::uint32_t mastered = g_call4(g, kMastered, record, 0x96u, sets, amount);
    g_st16(g, record + kMasteredCount, static_cast<std::uint16_t>(mastered));
    const std::uint32_t taught =
        g_call8(g, kTaught, record + kTaughtList, 0x20u, sets, static_cast<std::uint32_t>(g_ld8s(g, character)),
                record, static_cast<std::uint32_t>(g_ld16s(g, record + kMasteredCount)), 0u, 0u);
    g_st16(g, record + kTaughtCount, static_cast<std::uint16_t>(taught));
    std::cout << "[cheats] AP: " << given << " from the battle, " << amount << " with the cheat ("
              << static_cast<std::int16_t>(mastered) << " abilities mastered)" << std::endl;
}

struct Install {
    Install() {
        static const decomp::Override overrides[] = {
            {kWorkOutRewards, "cheat_ap_rewards", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
             &run_work_out_rewards},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
    }
} install_overrides;

} // namespace
} // namespace d012
