// The cheats the port offers on its menu's Cheats page, written as CWCheat
// codes and run by PortableKit's cheat engine as a PPSSPP cheat file would
// be (docs/port/cheats.md).
//
// The addresses and values are those of the public CWCheat list for
// ULUS-10566, but for the battle cheats, which find the fighters the game's
// own way (d012_battle_cheat_codes.hpp); they are facts about the game's
// memory. The names, the grouping
// and the choices are this port's. The European disc runs the same
// executable, so the same addresses hold there.
//
// Left out: the list's code patches (CP Doesn't Matter, Remove Bloom), which
// need the game's code changed; its widescreen and frame rate codes, which
// the port's own settings do; its Chaos codes; and its AP codes (0x09AEA568),
// which left the AP a Quick Battle gives unchanged. The AP row has no codes:
// the port gives the AP itself (d012_ap_cheat.cpp).
//
// MIT licence; see port/README.md.

#include "d012_cheats.hpp"

#include "d012_battle_cheat_codes.hpp"
#include "d012_summon_slot_rules.hpp"

#include "cheats/cheats.hpp"
#include "settings/settings.hpp"

#include <cstddef>
#include <cstring>
#include <iterator>

namespace d012 {
namespace {

using portablekit::BuiltinCheat;
using portablekit::CheatChoice;

// --- Money and points --------------------------------------------------------
// Gil is the word at 0x08BBFA00 (Customization shows it). The list's own
// 1,000,000 Gil code writes 0x08FF93F4 and 0x08FFB2D0 instead, which left the
// gil shown unchanged, so both choices write 0x08BBFA00.
constexpr CheatChoice kGil[] = {
    {"1,000,000", "0x203BFA00 0x000F4240"},
    {"99,999,999", "0x203BFA00 0x05F5E0FF"},
};
// KP is the word at 0x08BEE31C, in the save. The list's 1,000,000 KP code
// also writes 0x09692D14, in the game's pool, which other data uses in a
// battle; all three choices write only 0x08BEE31C.
constexpr CheatChoice kKp[] = {
    {"250", "0x003EE31C 0x000000FA"},
    {"1,000,000", "0x203EE31C 0x000F4240"},
    {"99,999,999", "0x203EE31C 0x05F5E0FF"},
};
// PP is the word at 0x08BCCB38, in the save (root + 0x1DE00, the last word
// of its first section); the PP Catalog copies it in when it opens and back
// when it closes. The list's PP code writes the catalog's copy, 0x096B2DC0,
// in the game's pool, which holds other data whenever the catalog is not
// open: the save being loaded, an animation of Yuna's while a battle loads
// (issue 20: the game crashed), the Brave numbers' list in a battle
// (issue 16). So both choices write 0x08BCCB38.
constexpr CheatChoice kPp[] = {
    {"20,000", "0x203CCB38 0x00004E20"},
    {"9,999,999", "0x203CCB38 0x0098967F"},
};
// AP: no codes. The battle's ending works out the AP and lists it on the
// results in one call, so a write every 77 ms comes too late; the port sets
// it as the game works it out (d012_ap_cheat.cpp). kApAmounts holds each
// choice's amount.
constexpr CheatChoice kAp[] = {{"100", nullptr}, {"999", nullptr}};
constexpr std::uint32_t kApAmounts[] = {100u, 999u};
static_assert(std::size(kAp) == std::size(kApAmounts));

// --- Items -------------------------------------------------------------------
// Each writes a count into every fourth byte: equipment from 0x08BBE41A,
// accessories from 0x08BBEEC6. The list's longer accessory code (672 entries)
// also reaches the summons, so the shorter one (589) is used. The list's
// summons code starts eight slots too late (d012_summon_slot_rules.hpp).
constexpr CheatChoice kEquipment[] = {{nullptr, "0x803BE41A 0x02A50004\n0x00000063 0x00000000"}};
constexpr CheatChoice kAccessories[] = {{nullptr, "0x803BEEC6 0x024D0004\n0x00000063 0x00000000"}};
constexpr CheatChoice kMalaBestia[] = {{nullptr, "0x003BF762 0x00000063\n0x003BF7AA 0x00000063\n"
                                                 "0x003BF122 0x00000063\n0x003BF7D6 0x00000063\n"
                                                 "0x003BF11A 0x00000063\n0x003BF116 0x00000063\n"
                                                 "0x003BF112 0x00000063\n0x003BF196 0x00000063\n"
                                                 "0x003BF0DE 0x00000063"}};
constexpr CheatChoice kSummons[] = {{nullptr, summon_slots::kAllSummonsCode}};
// Skill limit: no codes. The port keeps the second page of Skills itself
// (d012_skill_pages.cpp).
constexpr CheatChoice kSkillLimit[] = {{nullptr, nullptr}};

// --- Battle ------------------------------------------------------------------
// The codes are in d012_battle_cheat_codes.hpp.
constexpr CheatChoice kInfiniteHp[] = {{nullptr, battle_cheat_codes::kInfiniteHp}};
constexpr CheatChoice kMaxHp[] = {{nullptr, battle_cheat_codes::kMaxHp}};
constexpr CheatChoice kBravery[] = {{nullptr, battle_cheat_codes::kBravery}};
constexpr CheatChoice kInfiniteEx[] = {{nullptr, battle_cheat_codes::kInfiniteEx}};
constexpr CheatChoice kInfiniteAssist[] = {{nullptr, battle_cheat_codes::kInfiniteAssist}};
constexpr CheatChoice kLevel100[] = {{nullptr, battle_cheat_codes::kLevel100}};
constexpr CheatChoice kEnemyBravery[] = {{nullptr, battle_cheat_codes::kEnemyBravery}};
constexpr CheatChoice kEnemyMaxHp[] = {{nullptr, battle_cheat_codes::kEnemyMaxHp}};
constexpr CheatChoice kEnemyNoEx[] = {{nullptr, battle_cheat_codes::kEnemyNoEx}};
constexpr CheatChoice kEnemyNoAssist[] = {{nullptr, battle_cheat_codes::kEnemyNoAssist}};
constexpr CheatChoice kEnemyDefeat[] = {{nullptr, battle_cheat_codes::kEnemyDefeat}};

constexpr const char *kMoney = "Money and points";
constexpr const char *kItems = "Items";
constexpr const char *kBattle = "Battle";
constexpr const char *kOpponent = "Opponent";

constexpr BuiltinCheat kCheats[] = {
    {"gil", kMoney, "Gil", "Keeps your gil at this amount.", kGil},
    {"kp", kMoney, "KP", "Keeps your KP at this amount. Leave the KP shop and enter it again to see it.", kKp},
    {"pp", kMoney, "PP", "Keeps your PP at this amount. The PP Catalog shows it the next time you open it.", kPp},
    {"ap", kMoney, "AP", "Battles that give AP give this much.", kAp},
    {"equipment", kItems, "All equipment ×99", "Keeps 99 of every piece of equipment.", kEquipment},
    {"accessories", kItems, "All accessories ×99", "Keeps 99 of every accessory.", kAccessories},
    {"mala_bestia", kItems, "Mala Bestia accessories ×99", "Keeps 99 of each Mala Bestia accessory.", kMalaBestia},
    {"summons", kItems, "All summons", "Gives you every summonstone, with 10 uses left.", kSummons},
    {"skill_limit", kItems, "10 Skills", "Story Mode lets you hold 10 Skills, on two pages. L2 or R2 turns the page.",
     kSkillLimit},
    {"infinite_hp", kBattle, "Infinite HP", "Your HP stays full.", kInfiniteHp},
    {"max_hp", kBattle, "Max HP 999,999", "Your maximum HP is 999,999.", kMaxHp},
    {"bravery", kBattle, "Bravery 9,999", "Your Bravery stays at 9,999.", kBravery},
    {"infinite_ex", kBattle, "Infinite EX", "Your EX gauge stays full.", kInfiniteEx},
    {"infinite_assist", kBattle, "Infinite Assist", "Your Assist gauge stays full.", kInfiniteAssist},
    {"level_100", kBattle, "Level 100", "Raises the fighter you battle with to level 100. It stays at level 100 when the cheat is off.", kLevel100},
    {"enemy_bravery", kOpponent, "Bravery 0", "The opponent's Bravery stays at 0.", kEnemyBravery},
    {"enemy_max_hp", kOpponent, "Max HP 100", "The opponent's maximum HP is 100.", kEnemyMaxHp},
    {"enemy_no_ex", kOpponent, "No EX", "The opponent's EX gauge stays empty.", kEnemyNoEx},
    {"enemy_no_assist", kOpponent, "No Assist", "The opponent's Assist gauge stays empty.", kEnemyNoAssist},
    {"enemy_defeat", kOpponent, "Defeat with Select", "Press Select to defeat the opponent.", kEnemyDefeat},
};

} // namespace

std::span<const portablekit::BuiltinCheat> builtin_cheats() { return kCheats; }

std::uint32_t ap_after_battle() {
    if (!portablekit::settings::current().cheats || portablekit::cheats::paused_online()) return 0u;
    const portablekit::cheats::Overview overview = portablekit::cheats::overview();
    if (!overview.available) return 0u;
    for (std::size_t i = 0; i < std::size(kCheats) && i < overview.builtin.size(); ++i) {
        if (std::strcmp(kCheats[i].key, "ap") != 0) continue;
        const std::size_t choice = overview.builtin[i].choice;
        return choice >= 1u && choice <= std::size(kApAmounts) ? kApAmounts[choice - 1u] : 0u;
    }
    return 0u;
}

bool skill_pages_on() {
    if (!portablekit::settings::current().cheats || portablekit::cheats::paused_online()) return false;
    const portablekit::cheats::Overview overview = portablekit::cheats::overview();
    if (!overview.available) return false;
    for (std::size_t i = 0; i < std::size(kCheats) && i < overview.builtin.size(); ++i)
        if (std::strcmp(kCheats[i].key, "skill_limit") == 0) return overview.builtin[i].choice >= 1u;
    return false;
}

} // namespace d012
