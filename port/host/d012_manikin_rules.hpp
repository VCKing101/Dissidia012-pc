#pragma once

// The Manikin look: the rules, without the game (d012_manikin_look.cpp puts
// them in). Nothing here reads files, logs or needs PortableKit, so the tests
// (port/tests) check it. MIT licence; see port/README.md.
//
// A manikin is costume 0 of its character: objentry.bin has a fighter row
// P_<code>_ZAKO (and _ZAKO_A, _ZAKO_W) for each, which the game's own story
// and Labyrinth battles load (docs/research/roster-and-save.md, 1.2). The
// menus never offer it: the looks a character has are a mask (main
// 0x088D5E0C: bit 1 Normal, 2 Alt 1, 3 Alt 2, 4 to 12 DLC 1 to 9) and the
// next look after costume c is the next set bit above c, from 13 back to 1
// (main 0x088D608C), so bit 0 is never reached.

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace d012::manikin {

// The characters the game has a manikin of: each one with a manikin's name
// in its text (text/EN/mess_pk_chara_select_0.bin, table 0, strings 66 to
// 95, "False Hero" to "Fallacious Giant"). That is every fighter but Chaos
// (23, whose costume 0 is his first form) and Feral Chaos (32, whose _ZAKO
// row is his own model again).
[[nodiscard]] bool has_manikin(int character);

// The mask with the manikin added: only for a character the mask says is
// there to choose (not 0).
[[nodiscard]] std::uint32_t with_manikin(int character, std::uint32_t mask);

// The look after `current` for a mask with bit 0 set: the next look above
// it, then the manikin after the last, then Normal again. The game's own
// order otherwise.
[[nodiscard]] int next_look(int current, std::uint16_t mask);

// The look labels: a table of 26 strings in a menu text pack ("Normal",
// "Alt 1", "Normal", "Alt 1", "Alt 2", "DLC 1".."DLC 9", "Alt 2",
// "DLC 1".."DLC 9", and two dashes). The screens take string number
// table[costume] of it from a table of their own in the menu overlay; costume
// 0 points at Normal's. The port adds "Manikin" as string 26 and points
// costume 0 at it.
inline constexpr int kLabelStrings = 26;
inline constexpr int kManikinString = 26;

// A text pack ("mess pak": the magic, u16 version, u16 table count, u32 0,
// then {u32 offset, u32 size} per table, the tables 16-byte aligned with
// 0x77 between them) with `label` added to each look-label table in it.
// Nothing when the file is not a text pack or has no such table.
[[nodiscard]] std::optional<std::vector<std::uint8_t>> with_manikin_label(std::span<const std::uint8_t> pack,
                                                                           std::u16string_view label);

// The strings of one message table, as the game shows them (u16 count,
// u16 offset per string in 2-byte units, 0: none; each string a u16 byte
// length and 16-bit units, hidden by XOR with the game's key). For the
// tests and the log.
[[nodiscard]] std::vector<std::u16string> table_strings(std::span<const std::uint8_t> table);
// The tables of a text pack, as {offset, size}; empty when it is not one.
[[nodiscard]] std::vector<std::pair<std::uint32_t, std::uint32_t>> pack_tables(std::span<const std::uint8_t> pack);

// The menu overlay's tables of string numbers by costume (13 halfwords,
// costumes 0 to 12), as the disc has them: the character select screen's
// (menu 0x08A85B92, the pack's first block of labels) and two others' (menu
// 0x08AE45B4 and 0x08B0159C, the second block).
struct LabelTable {
    std::uint32_t address;
    std::uint16_t normal;  // the first string of its block: what costume 0 shows on the disc
    std::uint16_t alt1;
    std::uint16_t alt2;
    std::uint16_t dlc1;
};
inline constexpr LabelTable kLabelTables[] = {
    {0x08A85B92u, 0u, 1u, 4u, 5u},
    {0x08AE45B4u, 2u, 3u, 14u, 15u},
    {0x08B0159Cu, 2u, 3u, 14u, 15u},
};
inline constexpr int kCostumes = 13;
// The table as the disc has it.
[[nodiscard]] std::vector<std::uint16_t> disc_table(const LabelTable &table);

// The battle screen's portrait of a fighter is menu/JP/battle/chara_image/
// <its row's name>.gim (and <name>_2.gim for a second one), battle
// 0x08B21588. The disc has one for every look but the manikins'
// (P_<code>_ZAKO), so a player's manikin would have none: it is given its
// character's Normal one (P_<code>_1P), as the menus and the loading screen
// show. The path to read instead, or nothing for any other path (Feral
// Chaos's P_ORG210_ZAKO is on the disc).
[[nodiscard]] std::optional<std::string> portrait_instead(std::string_view path);

} // namespace d012::manikin
