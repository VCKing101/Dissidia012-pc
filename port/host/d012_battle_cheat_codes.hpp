#pragma once

// The Cheats page's battle cheats, as CWCheat codes (d012_cheats.cpp): no game
// here, so d012_tests runs them. MIT licence; see port/README.md.
//
// Each writes 32 bits in a fighter's status block. The way there is the
// game's own: the battle manager (the word at 0x089D6A10), its fighters at
// +0x38 (the player) and +0x3C (the opponent), each one's status block at
// +0x52C. Between battles the fighters are 0, and nothing is written.
//
// The public list's codes go through the word at 0x08B633B0 instead, which in
// most battles lies just after the opponent's status block but holds 0 in
// others, so they did nothing there (docs/port/cheats.md).

namespace d012::battle_cheat_codes {

// A pointer code (type 6): the value, then a 32-bit write at the offset from
// where the two steps on the third line end.
#define D012_PLAYER(value, field) "0x601D6A10 " value "\n0x00020002 " field "\n0x20000038 0x2000052C"
#define D012_OPPONENT(value, field) "0x601D6A10 " value "\n0x00020002 " field "\n0x2000003C 0x2000052C"

inline constexpr const char *kLevel100 = D012_PLAYER("0x00000064", "0x00000000");
inline constexpr const char *kInfiniteHp = D012_PLAYER("0x00000000", "0x00000008");
inline constexpr const char *kMaxHp = D012_PLAYER("0x000F423F", "0x0000000C");
inline constexpr const char *kBravery = D012_PLAYER("0x0000270F", "0x00000014");
inline constexpr const char *kInfiniteEx = D012_PLAYER("0x461C4000", "0x00000018");
inline constexpr const char *kInfiniteAssist = D012_PLAYER("0x461C4000", "0x0000001C");
inline constexpr const char *kEnemyMaxHp = D012_OPPONENT("0x00000064", "0x0000000C");
inline constexpr const char *kEnemyBravery = D012_OPPONENT("0x00000000", "0x00000014");
inline constexpr const char *kEnemyNoEx = D012_OPPONENT("0x00000000", "0x00000018");
inline constexpr const char *kEnemyNoAssist = D012_OPPONENT("0x00000000", "0x0000001C");
// Only while Select is held: the joker line skips the code's three lines
// otherwise (its low byte is that number less one).
inline constexpr const char *kEnemyDefeat = "0xD0000002 0x10000001\n" D012_OPPONENT("0x000F423F", "0x00000008");

#undef D012_PLAYER
#undef D012_OPPONENT

} // namespace d012::battle_cheat_codes
