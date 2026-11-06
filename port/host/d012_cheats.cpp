// The cheats the port offers on its menu's Cheats page, written as CWCheat
// codes and run by PortableKit's cheat engine as a PPSSPP cheat file would
// be (docs/port/cheats.md).
//
// The addresses and values are those of the public CWCheat list for
// ULUS-10566; they are facts about the game's memory. The names, the grouping
// and the choices are this port's. The European disc runs the same
// executable, so the same addresses hold there.
//
// Left out: the list's code patches (CP Doesn't Matter, Remove Bloom), which
// need the game's code changed; its widescreen and frame rate codes, which
// the port's own settings do; its Chaos codes; and its AP codes (0x09AEA568),
// which left the AP a Quick Battle gives unchanged.
//
// MIT licence; see port/README.md.

#include "d012_cheats.hpp"

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
constexpr CheatChoice kKp[] = {
    {"250", "0x003EE31C 0x000000FA"},
    {"1,000,000", "0x203EE31C 0x000F4240\n0x20E92D14 0x000F4240"},
    {"99,999,999", "0x203EE31C 0x05F5E0FF"},
};
constexpr CheatChoice kPp[] = {
    {"20,000", "0x20EB2DC0 0x00004E20"},
    {"9,999,999", "0x20EB2DC0 0x0098967F"},
};

// --- Items -------------------------------------------------------------------
// Each writes a count into every fourth byte: equipment from 0x08BBE41A,
// accessories from 0x08BBEEC6, summons from 0x08BBF8BD. The list's longer
// accessory code (672 entries) also reaches the summons, so the shorter one
// (589) is used.
constexpr CheatChoice kEquipment[] = {{nullptr, "0x803BE41A 0x02A50004\n0x00000063 0x00000000"}};
constexpr CheatChoice kAccessories[] = {{nullptr, "0x803BEEC6 0x024D0004\n0x00000063 0x00000000"}};
constexpr CheatChoice kMalaBestia[] = {{nullptr, "0x003BF762 0x00000063\n0x003BF7AA 0x00000063\n"
                                                 "0x003BF122 0x00000063\n0x003BF7D6 0x00000063\n"
                                                 "0x003BF11A 0x00000063\n0x003BF116 0x00000063\n"
                                                 "0x003BF112 0x00000063\n0x003BF196 0x00000063\n"
                                                 "0x003BF0DE 0x00000063"}};
constexpr CheatChoice kSummons[] = {{nullptr, "0x803BF8BF 0x003D0004\n0x00000003 0x00000000\n"
                                              "0x803BF8BD 0x003D0004\n0x0000000A 0x00000000"}};

// --- Battle ------------------------------------------------------------------
// Each writes 32 bits at the word at 0x08B633B0 minus an offset (pointer code
// type 5); the player's fields are 0xE20 bytes below the opponent's. The
// list follows each with a write of 0 to 0x0880052C, below the game, which
// these leave out.
#define D012_FIGHTER(value, offset) "0x603633B0 " value "\n0x00050001 " offset
constexpr CheatChoice kInfiniteHp[] = {{nullptr, D012_FIGHTER("0x00000000", "0x00000E58")}};
constexpr CheatChoice kMaxHp[] = {{nullptr, D012_FIGHTER("0x000F423F", "0x00000E54")}};
constexpr CheatChoice kBravery[] = {{nullptr, D012_FIGHTER("0x0000270F", "0x00000E4C")}};
constexpr CheatChoice kInfiniteEx[] = {{nullptr, D012_FIGHTER("0x461C4000", "0x00000E48")}};
constexpr CheatChoice kInfiniteAssist[] = {{nullptr, D012_FIGHTER("0x461C4000", "0x00000E44")}};
constexpr CheatChoice kLevel100[] = {{nullptr, D012_FIGHTER("0x00000064", "0x00000E60")}};
constexpr CheatChoice kEnemyBravery[] = {{nullptr, D012_FIGHTER("0x00000000", "0x0000002C")}};
constexpr CheatChoice kEnemyMaxHp[] = {{nullptr, D012_FIGHTER("0x00000064", "0x00000034")}};
constexpr CheatChoice kEnemyNoEx[] = {{nullptr, D012_FIGHTER("0x00000000", "0x00000028")}};
constexpr CheatChoice kEnemyNoAssist[] = {{nullptr, D012_FIGHTER("0x00000000", "0x00000024")}};
// Only while Select is held: the joker line skips the next two otherwise.
constexpr CheatChoice kEnemyDefeat[] = {
    {nullptr, "0xD0000001 0x10000001\n" D012_FIGHTER("0x000F423F", "0x00000038")}};
#undef D012_FIGHTER

constexpr const char *kMoney = "Money and points";
constexpr const char *kItems = "Items";
constexpr const char *kBattle = "Battle";
constexpr const char *kOpponent = "Opponent";

constexpr BuiltinCheat kCheats[] = {
    {"gil", kMoney, "Gil", "Keeps your gil at this amount.", kGil},
    {"kp", kMoney, "KP", "Keeps your KP at this amount. Leave the KP shop and enter it again to see it.", kKp},
    {"pp", kMoney, "PP", "Keeps your PP at this amount. Leave the PP Catalog and open it again to see it.", kPp},
    {"equipment", kItems, "All equipment ×99", "Keeps 99 of every piece of equipment.", kEquipment},
    {"accessories", kItems, "All accessories ×99", "Keeps 99 of every accessory.", kAccessories},
    {"mala_bestia", kItems, "Mala Bestia accessories ×99", "Keeps 99 of each Mala Bestia accessory.", kMalaBestia},
    {"summons", kItems, "All summons", "Gives you every summonstone, with 10 uses left.", kSummons},
    {"infinite_hp", kBattle, "Infinite HP", "Your HP stays full.", kInfiniteHp},
    {"max_hp", kBattle, "Max HP 999,999", "Your maximum HP is 999,999.", kMaxHp},
    {"bravery", kBattle, "Bravery 9,999", "Your Bravery stays at 9,999.", kBravery},
    {"infinite_ex", kBattle, "Infinite EX", "Your EX gauge stays full.", kInfiniteEx},
    {"infinite_assist", kBattle, "Infinite Assist", "Your Assist gauge stays full.", kInfiniteAssist},
    {"level_100", kBattle, "Level 100", "Your fighter is level 100.", kLevel100},
    {"enemy_bravery", kOpponent, "Bravery 0", "The opponent's Bravery stays at 0.", kEnemyBravery},
    {"enemy_max_hp", kOpponent, "Max HP 100", "The opponent's maximum HP is 100.", kEnemyMaxHp},
    {"enemy_no_ex", kOpponent, "No EX", "The opponent's EX gauge stays empty.", kEnemyNoEx},
    {"enemy_no_assist", kOpponent, "No Assist", "The opponent's Assist gauge stays empty.", kEnemyNoAssist},
    {"enemy_defeat", kOpponent, "Defeat with Select", "Press Select to defeat the opponent.", kEnemyDefeat},
};

} // namespace

std::span<const portablekit::BuiltinCheat> builtin_cheats() { return kCheats; }

} // namespace d012
