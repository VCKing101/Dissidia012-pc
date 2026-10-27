// Local versus: the Battle Mode row "Quick Battle (Player vs Player)"
// (docs/port/local-versus.md, sections 4 and 7.6).
//
// The start menu (menu overlay) builds Battle Mode from tables in the
// overlay's data, through the submenu table at menu 0x089E7530 (0x18 bytes
// a submenu): +0x0C its parts (a layout instance each, an animation, the
// parent part and the parent's slot), +0x10 the texts on them, +0x14 its rows
// (two parts, the hand pointer's slot and a row type). Every part is an
// instance of the menu's layout 9, and every text a text object. The row is
// one more row of the same kind: two parts made and attached as the game
// makes the others, texts made from strings and put on them, and the parts
// and rows tables copied below the EBOOT with the row added and the submenu
// pointed at them.
//
// The row's type is 19, whose entry in the row-type table (menu 0x089E7488,
// 8 bytes a type) is the start of the submenu table: not enabled, help text
// 0. Battle Mode's own handler (menu 0x089E4A60) therefore only plays the
// "not allowed" sound for the row, and the port takes it from there; its help
// is shown and hidden through text 0, which no other Battle Mode row uses.
//
// Nothing in the menu overlay can be overridden (the harness takes the main
// module and the battle overlay), so every hook is a main function the menu
// calls, told apart by its caller:
//
//   - main 0x088D3B88 (make parts from a table), at the menu's start (menu
//     0x089E4328): the submenu tables go back to the game's;
//   - main 0x088D428C (put texts on parts by a table), at the same start with
//     Battle Mode's text table: the row's parts and texts are made;
//   - main 0x088D3C24 (release parts by a table), at the menu's end (menu
//     0x089E17F8): the row's parts and texts are released;
//   - main 0x088D46E4 (a cursor), when Battle Mode opens (menu 0x089E45E8):
//     one more row, and the submenu pointed at the row's tables;
//   - main 0x0887B924 / 0x0887B944 (show / hide a text), from the help line's
//     swap (menu 0x089E2A24): the row's help, and its highlight;
//   - main 0x088D4BF4 (a menu sound), from Battle Mode's handler: the row
//     chosen; Quick Battle (1-on-1) with versus on, or "Connect a second
//     controller";
//   - main 0x088D3CF0, 0x088D3F00, 0x088D44EC and 0x088D428C, handed the
//     battle setup's tables: in versus, the setup without the CPU rows (see
//     "the battle setup" below).
//
// MIT licence; see port/README.md.

#include "d012_decomp_harness.hpp"
#include "d012_versus.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace d012::versus_menu {
namespace {

// menu overlay
constexpr std::uint32_t kSubmenuParts = 0x089E753Cu;  // Battle Mode's tables
constexpr std::uint32_t kSubmenuTexts = 0x089E7540u;
constexpr std::uint32_t kSubmenuRows = 0x089E7544u;
constexpr std::uint32_t kGameParts3 = 0x089E682Au;  // three rows
constexpr std::uint32_t kGameParts4 = 0x089E6862u;  // four (the fourth unlocked)
constexpr std::uint32_t kGameTexts = 0x089E6CCEu;
constexpr std::uint32_t kGameRows = 0x089E6EA4u;
constexpr std::uint32_t kPartsMade = 0x089E6606u;  // every part of the start menu
constexpr std::uint32_t kMenuStart = 0x089E4328u, kMenuStartEnd = 0x089E45E8u;
constexpr std::uint32_t kMainRows = 0x089E45E8u, kMainRowsEnd = 0x089E4A60u;
constexpr std::uint32_t kBattleMode = 0x089E4A60u, kBattleModeEnd = 0x089E4E58u;
constexpr std::uint32_t kHelpSwap = 0x089E2A24u, kHelpSwapEnd = 0x089E2AB8u;
constexpr std::uint32_t kDestination = 0x09B6D838u;
constexpr std::uint32_t kQuickBattle = 2u;  // the destination of Quick Battle (1-on-1)
// The start menu's state (menu +0x140)
constexpr std::uint32_t kStateMode = 0x0u;  // 4: leaving for the destination
constexpr std::uint32_t kStateLayouts = 0x8u;
constexpr std::uint32_t kStateParts = 0x408u;
constexpr std::uint32_t kStateTexts = 0x8B8u;
constexpr std::uint32_t kStateMainRow = 0xA2Cu;
constexpr std::uint32_t kStateCursor = 0xA58u;  // +4 the row, +0xC the last row
constexpr std::uint32_t kStateRow = 0xA5Cu;
constexpr std::uint32_t kStateSubmenu = 0xA88u;
constexpr std::uint32_t kBattleModeSubmenu = 1u;
constexpr std::uint32_t kParent = 0x16u;  // Battle Mode's frame, the rows' parent
constexpr std::uint32_t kRowType = 19u;
// main
constexpr std::uint32_t kPartClone = 0x088DAA68u;  // (layout) -> part, hidden
constexpr std::uint32_t kPartAnimate = 0x088D9F5Cu;  // (part, animation, 1)
constexpr std::uint32_t kPartAttach = 0x088D3AE8u;  // (parts, parent, slot, child)
constexpr std::uint32_t kPartHide = 0x088D9CFCu;
constexpr std::uint32_t kTextAttach = 0x088D40BCu;  // (parts, texts, text, flags, part, locator)
constexpr std::uint32_t kTextFromString = 0x0887D868u;  // (font, 0, utf16, -1, 0, 0, 0, 0)
constexpr std::uint32_t kTextCopy = 0x0887E534u;
constexpr std::uint32_t kTextSetUp = 0x0887BA04u;
constexpr std::uint32_t kCursorClamp = 0x088D4744u;
constexpr std::uint32_t kPlaySound = 0x088A24D4u;  // (0, sound, 1, 0)
constexpr std::uint32_t kSounds = 0x089CBE60u;
constexpr std::uint32_t kFont = 0x089D6998u;  // -> the text font
constexpr std::uint32_t kSaveFlags = 0x089D726Cu;  // -> the save's flag words
constexpr std::uint32_t kSoundRefused = 4u, kSoundChosen = 1u;
// The row's parts, in ids the start menu leaves free (its parts end at 70).
// Its label on the label part and the label's copy on the bar part (locator
// 1: the label as it reads on the lit bar) are menu texts 0x50 and 0x51,
// shown and hidden with the submenu through its texts table as the other
// rows' are (the menu's own run to 0x4F, 0x52 up are made as the menu goes,
// and it releases 0 to 0x58 at its end). Texts on these parts draw only from
// that array; the game's rows have a third copy (bar, locator 0) for which
// there is no free id. The help is kept apart, in a word of its own below the
// EBOOT, and put on the help line through it.
constexpr std::uint32_t kPartLabel = 71u, kPartBar = 72u;
constexpr std::uint32_t kTextLabel = 0x50u, kTextBarLit = 0x51u;
constexpr std::uint32_t kRowPrivate = 0x08800A80u;
constexpr std::uint32_t kPrivateHelp = 0u, kPrivateCount = 1u;
constexpr std::uint32_t kPartFree = 0x088DA060u;  // (part)
constexpr std::uint32_t kLayoutFree = 0x088DAB44u;  // (layout), with its parts
constexpr std::uint32_t kTextFree = 0x0887BA5Cu;  // (text)
constexpr std::uint32_t kHelpPart = 0x32u;  // the help line, where every row's help goes (menu 0x089E6B66)
constexpr std::uint32_t kHelpFlags = 0x12u;
// Below the EBOOT (the split view's task and cursor are lower down).
constexpr std::uint32_t kRowParts = 0x08800800u;
constexpr std::uint32_t kRowTexts = 0x08800880u;
constexpr std::uint32_t kRowRows = 0x08800920u;
constexpr std::uint32_t kLabelString = 0x08800960u;
constexpr std::uint32_t kHelpString = 0x088009C0u;

const char kLabel[] = "Quick Battle (Player vs Player)";
const char kHelp[] = "Fight a friend on a second controller, on a split screen.";

struct State {
    gaddr menu = 0;   // the start menu's state the row was made for
    bool four = false;  // Battle Mode has four rows of its own
    std::uint32_t row = 0;  // the row's index
};

State &state() {
    static State s;
    return s;
}

std::uint32_t ld16(Guest *g, gaddr a) { return static_cast<std::uint16_t>(g_ld16s(g, a)); }

bool from(const decomp::CallState &in, std::uint32_t begin, std::uint32_t end) {
    const std::uint32_t ra = in.gpr[31];
    return ra >= begin && ra < end;
}

// The menu overlay is in the overlay window, with Battle Mode's own rows
// (the battle overlay shares the addresses).
bool menu_resident(Guest *g) { return g_ld32(g, kGameRows) == 0x001B0017u && g_ld32(g, kGameRows + 4u) == 0x00070004u; }

// The start menu the row was made for is up, in Battle Mode.
bool in_battle_mode(Guest *g) {
    const State &s = state();
    return s.menu != 0u && menu_resident(g) && g_ld32(g, s.menu + kStateSubmenu) == kBattleModeSubmenu;
}

void write_utf16(Guest *g, gaddr at, const char *text) {
    for (std::size_t i = 0;; ++i) {
        g_st16(g, at + 2u * static_cast<std::uint32_t>(i), static_cast<std::uint8_t>(text[i]));
        if (text[i] == '\0') break;
    }
}

std::uint32_t text_from_string(Guest *g, gaddr string) {
    const std::uint32_t text = g_call8(g, kTextFromString, g_ld32(g, kFont), 0u, string, 0xFFFFFFFFu, 0u, 0u, 0u, 0u);
    if (text != 0u) {
        g_call1(g, kTextSetUp, text);
        g_st32(g, text + 0x10u, g_ld32(g, text + 0x10u) & ~0x400u);  // hidden, as the menu makes its own
    }
    return text;
}

// Copies 8-byte entries from `game` until `end` says the table is over, adds
// `added`, and ends the copy as the game's tables end.
template <typename End>
void copy_table(Guest *g, gaddr copy, gaddr game, End end, const std::uint16_t (*added)[4], std::size_t count,
                std::uint32_t limit = 0xFFFFFFFFu) {
    std::uint32_t n = 0;
    for (gaddr p = game; !end(p) && n < limit; p += 8u, ++n) {
        g_st32(g, copy + 8u * n, g_ld32(g, p));
        g_st32(g, copy + 8u * n + 4u, g_ld32(g, p + 4u));
    }
    for (std::size_t i = 0; i < count; ++i, ++n)
        for (std::uint32_t k = 0; k < 4u; ++k) g_st16(g, copy + 8u * n + 2u * k, added[i][k]);
    g_st32(g, copy + 8u * n, 0xFFFFFFFFu);
    g_st32(g, copy + 8u * n + 4u, 0xFFFFFFFFu);
}

// The row's parts and texts, made as the menu makes its own (main
// 0x088D3B88, then the attachments of menu 0x089E20AC, then main 0x088D3F7C
// and 0x088D428C), and its copies of Battle Mode's tables.
void make_row(Guest *g, gaddr menu) {
    State &s = state();
    const gaddr parts = menu + kStateParts;
    s.four = g_ld32(g, kSubmenuParts) == kGameParts4;
    s.row = s.four ? 4u : 3u;
    // The fourth row's slot when there are three, the one below it otherwise.
    const std::uint32_t slot = s.four ? 0u : 1u;
    const gaddr layout = g_ld32(g, menu + kStateLayouts + 9u * 4u);
    const std::uint32_t looks[2][2] = {{kPartLabel, 0x12u}, {kPartBar, 0x11u}};
    for (const auto &[id, look] : looks) {
        const gaddr part = g_call1(g, kPartClone, layout);
        g_st32(g, parts + 4u * id, part);
        if (part == 0u) return;
        g_call3(g, kPartAnimate, part, look, 1u);
        g_call4(g, kPartAttach, parts, kParent, slot, id);
        g_call1(g, kPartHide, part);
        g_st32(g, part + 0x30u, 0u);
        g_st32(g, part + 0x34u, 0u);
        g_st32(g, part + 0x38u, 0u);
    }
    write_utf16(g, kLabelString, kLabel);
    write_utf16(g, kHelpString, kHelp);
    const gaddr texts = menu + kStateTexts;
    const std::uint32_t label = text_from_string(g, kLabelString);
    g_st32(g, texts + 4u * kTextLabel, label);
    const std::uint32_t copy = label != 0u ? g_call1(g, kTextCopy, label) : 0u;
    if (copy != 0u) {
        g_call1(g, kTextSetUp, copy);
        g_st32(g, copy + 0x10u, g_ld32(g, copy + 0x10u) & ~0x400u);
    }
    g_st32(g, texts + 4u * kTextBarLit, copy);
    const std::uint32_t on_parts[2][4] = {{kTextLabel, 9u, kPartLabel, 0u}, {kTextBarLit, 9u, kPartBar, 1u}};
    for (const auto &e : on_parts)
        if (g_ld32(g, texts + 4u * e[0]) != 0u) g_call8(g, kTextAttach, parts, texts, e[0], e[1], e[2], e[3], 0u, 0u);
    const std::uint32_t help = text_from_string(g, kHelpString);
    g_st32(g, kRowPrivate + 4u * kPrivateHelp, help);
    // Putting a text on a part shows it; the help line shows the row's help
    // only while the row is chosen.
    if (help != 0u) {
        g_call8(g, kTextAttach, parts, kRowPrivate, kPrivateHelp, kHelpFlags, kHelpPart, 0u, 0u, 0u);
        g_st32(g, help + 0x10u, g_ld32(g, help + 0x10u) & ~0x400u);
    }

    const std::uint16_t row_parts[2][4] = {{static_cast<std::uint16_t>(kPartLabel), 0x12u, static_cast<std::uint16_t>(kParent),
                                            static_cast<std::uint16_t>(slot)},
                                           {static_cast<std::uint16_t>(kPartBar), 0x11u, static_cast<std::uint16_t>(kParent),
                                            static_cast<std::uint16_t>(slot)}};
    copy_table(g, kRowParts, g_ld32(g, kSubmenuParts), [&](gaddr p) { return ld16(g, p) == 0xFFFFu; }, row_parts, 2u);
    const std::uint16_t row_texts[2][4] = {{static_cast<std::uint16_t>(kTextLabel), 9u, static_cast<std::uint16_t>(kPartLabel), 0u},
                                           {static_cast<std::uint16_t>(kTextBarLit), 9u, static_cast<std::uint16_t>(kPartBar), 1u}};
    copy_table(g, kRowTexts, kGameTexts, [&](gaddr p) { return ld16(g, p + 4u) == 0xFFFFu; }, row_texts, 2u);
    const std::uint16_t row[1][4] = {{kPartLabel, kPartBar, static_cast<std::uint16_t>(slot),
                                      static_cast<std::uint16_t>(kRowType)}};
    copy_table(g, kRowRows, kGameRows, [&](gaddr p) { return ld16(g, p + 6u) == 0xFFFFu; }, row, 1u, s.row);
    s.menu = menu;
    std::cout << "[versus] Battle Mode row made (" << (s.four ? "after four rows" : "after three rows") << "), texts 0x"
              << std::hex << label << " 0x" << copy << " 0x" << help << std::dec << std::endl;
}

// Battle Mode on the row's tables (while the start menu lasts).
void point_at_row(Guest *g) {
    g_st32(g, kSubmenuParts, kRowParts);
    g_st32(g, kSubmenuTexts, kRowTexts);
    g_st32(g, kSubmenuRows, kRowRows);
}

// The row's parts released, as the menu releases its own when it ends (menu
// 0x089E17F8); its label texts go with the menu's texts afterwards.
void free_row_parts(Guest *g, gaddr parts) {
    for (const std::uint32_t id : {kPartLabel, kPartBar}) {
        const gaddr part = g_ld32(g, parts + 4u * id);
        if (part != 0u) g_call1(g, kPartFree, part);
        g_st32(g, parts + 4u * id, 0u);
    }
}

// The row's own texts released, after every part (their parts listed them).
void free_row_texts(Guest *g) {
    for (std::uint32_t i = 0; i < kPrivateCount; ++i) {
        const gaddr text = g_ld32(g, kRowPrivate + 4u * i);
        if (text != 0u) g_call1(g, kTextFree, text);
        g_st32(g, kRowPrivate + 4u * i, 0u);
    }
    state().menu = 0u;
}

// ---- the hooks ---------------------------------------------------------------

// main 0x088D3B88 (layouts, parts, table, first layout): the parts of `table`,
// each an instance of its layout.
void run_make_parts(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr layouts = in.gpr[4], parts = in.gpr[5], table = in.gpr[6];
    const std::uint32_t first = in.gpr[7];
    if (table == kPartsMade && from(in, kMenuStart, kMenuStartEnd) && menu_resident(g)) {
        // A start menu begins: Battle Mode's tables are the game's until the
        // row is made again, and versus is off.
        if (g_ld32(g, kSubmenuTexts) == kRowTexts) g_st32(g, kSubmenuTexts, kGameTexts);
        if (g_ld32(g, kSubmenuRows) == kRowRows) g_st32(g, kSubmenuRows, kGameRows);
        // Three or four rows by the save's flag, as the menu's set-up
        // chooses (menu 0x089E3DB8).
        if (g_ld32(g, kSubmenuParts) == kRowParts) {
            const gaddr flags = g_ld32(g, kSaveFlags);
            const bool four = flags != 0u && (g_ld32(g, flags) & 0x400000u) != 0u;
            g_st32(g, kSubmenuParts, four ? kGameParts4 : kGameParts3);
        }
        state().menu = 0u;
        versus::set_chosen(false);
        versus::note_start_menu();
    }
    for (gaddr p = table;; p += 4u) {
        const std::uint32_t id = ld16(g, p);
        if (id == 0xFFFFu) break;
        const std::uint32_t index = ld16(g, p + 2u);
        const gaddr layout = g_ld32(g, layouts + ((index + first) & 0xFFFFu) * 4u);
        g_st32(g, parts + id * 4u, g_call2(g, kPartClone, layout, index));
    }
}

// main 0x088D428C (parts, texts, table, reset): the texts of `table` put on
// their parts, the parts' text lists emptied first when `reset`.
std::uint32_t setup_table(Guest *g, std::uint32_t table);

void run_attach_texts(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr parts = in.gpr[4], texts = in.gpr[5], table = setup_table(g, in.gpr[6]);
    if ((in.gpr[7] & 0xFFu) != 0u) {
        for (gaddr p = table; ld16(g, p + 4u) != 0xFFFFu; p += 8u) {
            const gaddr part = g_ld32(g, parts + ld16(g, p + 4u) * 4u);
            g_st32(g, part + 0x34u, 0u);
            g_st32(g, part + 0x30u, 0u);
            g_st32(g, part + 0x38u, 0u);
        }
    }
    for (gaddr p = table; ld16(g, p + 4u) != 0xFFFFu; p += 8u)
        if (g_ld32(g, texts + ld16(g, p) * 4u) != 0u)
            g_call8(g, kTextAttach, parts, texts, ld16(g, p), ld16(g, p + 2u), ld16(g, p + 4u), ld16(g, p + 6u), 0u, 0u);
    if (table == kGameTexts && from(in, kMenuStart, kMenuStartEnd) && menu_resident(g))
        make_row(g, parts - kStateParts);
}

// main 0x088D3C24 (layouts, parts, table, count): the parts of `table`
// released. The menu's end (menu 0x089E17F8) releases the row's first.
void run_free_parts(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr parts = in.gpr[5], table = in.gpr[6];
    if (table == kPartsMade && menu_resident(g) && state().menu != 0u && parts == state().menu + kStateParts)
        free_row_parts(g, parts);
    for (gaddr p = table;; p += 4u) {
        const std::uint32_t id = ld16(g, p);
        if (id == 0xFFFFu) break;
        const gaddr part = g_ld32(g, parts + id * 4u);
        if (part == 0u) continue;
        g_call1(g, kPartFree, part);
        g_st32(g, parts + id * 4u, 0u);
    }
}

// main 0x088D3A04 (layouts): the 256 layouts released, last first, and with
// them their parts. Both of the menu's ends call it before releasing the
// texts (menu 0x089E133C, leaving for a destination, and 0x089E17F8); the
// row's help goes then, or the menu waits for ever for its font to have no
// texts left (menu 0x089E1428).
void run_free_layouts(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr layouts = in.gpr[4];
    for (std::uint32_t i = 256u; i-- > 0u;) {
        const gaddr layout = g_ld32(g, layouts + 4u * i);
        if (layout == 0u) continue;
        g_call1(g, kLayoutFree, layout);
        g_st32(g, layouts + 4u * i, 0u);
    }
    if (state().menu != 0u && layouts == state().menu + kStateLayouts && menu_resident(g)) free_row_texts(g);
}

// main 0x088D46E4 (cursor, row, a2, last row, t0, t1).
// Player 2's level on the setup screen. The setup's start (menu 0x08AA6F0C,
// on an object with its setup state at +0x1EE4) works the
// opponent's level out from the CPU level setting (menu 0x08AB13A0, into
// state +0x1E4C; menu 0x08AA7C3C, which would draw it too, does nothing in
// this build) and puts it on the right name plate (menu 0x08AA6148: the
// digits by menu 0x08AB0A08 on the state's parts at +0x10, the digit count's
// animation on the part at +0xD8 from the table at 0x08AA885E). In versus
// player 2's fighter is player 1's saved one (d012_versus.cpp), so after
// that, at the start's cursor call (cursor state +0x1DF0), the plate is given
// that record's level as the player's plate gets player 1's (menu
// 0x08AA56C8: main 0x088CB164 and 0x088CBAD0).
constexpr std::uint32_t kSetupStart = 0x08AA6F0Cu;
constexpr std::uint32_t kSetupStartSize = 1640u;

void show_player2_level(Guest *g, gaddr cursor) {
    const gaddr setup = cursor - 0x1DF0u;
    // The versus setup's rows (run_setup_attach): the state it is.
    if (g_ld32(g, setup + 0x71Cu) != 0x08AA87A8u) return;
    const std::int32_t character = g_ld32s(g, setup + 0x594u);
    if (character <= 0 || character == 0x17) return;
    const gaddr record = g_call2(g, 0x088CB164u, g_ld32(g, 0x089D712Cu) + 0xF58u, static_cast<std::uint32_t>(character));
    const std::uint32_t level = g_call1(g, 0x088CBAD0u, record);
    if (level == 0u || level > 100u || level == g_ld32(g, setup + 0x1E4Cu)) return;
    g_st32(g, setup + 0x1E4Cu, level);
    const std::uint32_t digits = g_call4(g, 0x08AB0A08u, setup + 0x10u, level, 0x08AA7EF3u, 0u);
    if (digits < 1u || digits > 3u) return;
    g_st32(g, setup + 0x1E6Cu, digits - 1u);
    g_call3(g, 0x088D9F5Cu, g_ld32(g, setup + 0xD8u), ld16(g, 0x08AA885Eu + (digits - 1u) * 2u), 1u);
    std::cout << "[versus] player 2's level on the setup screen: " << level << std::endl;
}

void run_cursor(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr cursor = in.gpr[4];
    std::int32_t last = static_cast<std::int32_t>(in.gpr[7]);
    if (last < 0) last = 0;
    const State &s = state();
    if (s.menu != 0u && cursor == s.menu + kStateCursor && from(in, kMainRows, kMainRowsEnd) && menu_resident(g) &&
        g_ld32(g, s.menu + kStateMainRow) == kBattleModeSubmenu) {
        last = static_cast<std::int32_t>(s.row);
        point_at_row(g);
    }
    g_st32(g, cursor, 8u);
    g_st32(g, cursor + 4u, in.gpr[5]);
    g_st32(g, cursor + 8u, in.gpr[6]);
    g_st32(g, cursor + 12u, static_cast<std::uint32_t>(last));
    g_st32(g, cursor + 16u, in.gpr[8]);
    g_st32(g, cursor + 20u, in.gpr[8]);
    g_st32(g, cursor + 24u, in.gpr[9]);
    g_st32(g, cursor + 28u, 0u);
    g_st32(g, cursor + 40u, 0u);
    g_call1(g, kCursorClamp, cursor);
    if (versus::active() && from(in, kSetupStart, kSetupStart + kSetupStartSize)) show_player2_level(g, cursor);
}

// The row's highlight, as menu 0x089E56F8 lights a row and puts it out.
void light_row(Guest *g, bool lit) {
    const gaddr bar = g_ld32(g, state().menu + kStateParts + 4u * kPartBar);
    if (bar == 0u) return;
    if (lit) {
        g_call3(g, kPartAnimate, bar, 0xFu, 0u);
        g_st16(g, bar + 0xEu, 0x10u);
    } else {
        g_call3(g, kPartAnimate, bar, 0x11u, 0u);
    }
}

// The help line's swap shows and hides text 0 for the row: its help instead.
gaddr help_for_row(Guest *g, const decomp::CallState &in, gaddr text) {
    static const bool trace = std::getenv("D012_TRACE_VERSUS_MENU") != nullptr;
    if (trace && menu_resident(g) && state().menu != 0u && from(in, kHelpSwap, kHelpSwapEnd))
        std::printf("[versus-menu] help swap text 0x%08X from 0x%08X, submenu %u, text 0 0x%08X, row's help 0x%08X\n",
                    text, in.gpr[31], g_ld32(g, state().menu + kStateSubmenu), g_ld32(g, state().menu + kStateTexts),
                    g_ld32(g, kRowPrivate + 4u * kPrivateHelp));
    if (!from(in, kHelpSwap, kHelpSwapEnd) || !in_battle_mode(g)) return 0u;
    if (text != g_ld32(g, state().menu + kStateTexts)) return 0u;
    return g_ld32(g, kRowPrivate + 4u * kPrivateHelp);
}

// main 0x0887B924 (text): shown.
// D012_TRACE_TEXTS=1: in a versus battle, each text shown or hidden, with the
// code that asked (for finding a HUD piece made of texts).
void trace_text(const char *what, const decomp::CallState &in) {
    static const bool on = [] {
        const char *text = std::getenv("D012_TRACE_TEXTS");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    static std::uint32_t told = 0;
    if (!on || !versus::split_recently() || told >= 4000u) return;
    ++told;
    std::printf("[text] %s %08X from %08X\n", what, in.gpr[4], in.gpr[31]);
}

void run_text_show(Guest *g, const decomp::CallState &in, GuestRegs &) {
    trace_text("show", in);
    gaddr text = in.gpr[4];
    if (const gaddr help = help_for_row(g, in, text); help != 0u) {
        text = help;
        light_row(g, true);
    }
    g_st32(g, text + 0x10u, g_ld32(g, text + 0x10u) | 0x400u);
}

// main 0x0887B900 (text, id): the text's string (+8), as the game does it; for
// D012_TRACE_TEXTS, logged with the caller.
void run_text_string(Guest *g, const decomp::CallState &in, GuestRegs &) {
    static const bool on = [] {
        const char *text = std::getenv("D012_TRACE_TEXTS");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    static std::uint32_t told = 0;
    if (on && versus::split_recently() && told < 4000u) {
        ++told;
        std::printf("[text] string %08X = %u from %08X\n", in.gpr[4], in.gpr[5] & 0xFFFFu, in.gpr[31]);
    }
    g_st16(g, in.gpr[4] + 8u, static_cast<std::uint16_t>(in.gpr[5]));
}

// main 0x0887AD78 (message file, ?, message, ...): a text made from a
// message; for D012_TRACE_TEXTS, logged with the caller and the message.
void run_message_text(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    decomp::call_original(g, 0x0887AD78u, regs);
    out.v0 = regs.v0;
    static const bool on = [] {
        const char *text = std::getenv("D012_TRACE_TEXTS");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    static std::uint32_t told = 0;
    if (on && versus::split_recently() && told < 4000u) {
        ++told;
        std::printf("[text] made %08X from message file %08X, %08X, message %u, from %08X\n", regs.v0, in.gpr[4],
                    in.gpr[5], in.gpr[6], in.gpr[31]);
    }
}

// main 0x0887B944 (text): hidden.
void run_text_hide(Guest *g, const decomp::CallState &in, GuestRegs &) {
    trace_text("hide", in);
    gaddr text = in.gpr[4];
    if (const gaddr help = help_for_row(g, in, text); help != 0u) {
        text = help;
        light_row(g, false);
    }
    g_st32(g, text + 0x10u, g_ld32(g, text + 0x10u) & ~0x400u);
}

// main 0x088D4BF4 (sound) -> the voice, or 0.
void run_sound(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    std::uint32_t sound = in.gpr[4];
    const State &s = state();
    if (sound == kSoundRefused && from(in, kBattleMode, kBattleModeEnd) && in_battle_mode(g) &&
        g_ld32(g, s.menu + kStateRow) == s.row) {
        if (versus::second_pad_connected()) {
            // Quick Battle (1-on-1), as its own row leaves for it.
            g_st32(g, kDestination, kQuickBattle);
            g_st32(g, s.menu + kStateMode, 4u);
            versus::set_chosen(true);
            sound = kSoundChosen;
        } else {
            versus::ask_for_second_pad();
        }
    }
    out.v0 = sound < 0x29u ? g_call4(g, kPlaySound, 0u, g_ld32(g, kSounds + sound * 4u), 1u, 0u) : 0u;
}

// ---- the battle setup ----------------------------------------------------------
//
// Quick Battle's setup screen (the character room's setup object, menu
// 0x08AA5B6C and around) picks its rows by mode when it starts (menu
// 0x08AA6F0C): a table of display rows to settings at state +0x71C and a
// count at +0x720, a layout of the rows' parts, and a table of the settings'
// texts on them. Quick Battle has Stage, Strength (CPU), Level (CPU), Type
// (CPU) and Rules (layout 0x08AA83E0, texts 0x08AA7EF8, rows 0x08AA8790);
// mode 2 (state +0x58C) has Stage, Rules and Judgment (layout 0x08AA8588,
// texts 0x08AA8048, rows 0x08AA87A8: four rows with Enter Battle). In versus
// the Quick Battle setup gets mode 2's rows: the main functions the tables
// are handed to (attach parts, put texts on parts, place texts) take a row
// layout of both (below), a texts table of Quick Battle's first entry (player
// 1's name plate, as Quick Battle draws it) and mode 2's rows, and the name
// plates with a "GUEST" badge on the right one instead of "CPU"; the rows
// follow right after the start's last attachment (the name plates, layout
// 0x08AA86F0).
//
// A layout entry is four u16: part, animation, parent part, parent locator.
// Mode 2's layout has its own header (part 4, animation 6, with "L View
// Ruleset") and a ruleset icon (part 6) under player 1's level; the versus
// layout keeps Quick Battle's header and its first six entries, with mode 2's
// three rows on Quick Battle's first three row places (locators 5, 4, 3), and
// then mode 2's entries from part 0xE on (the rows' own pieces). The name
// plates' entry for part 0x3D takes mode 2's animation 1, a "GUEST" badge,
// instead of Quick Battle's 2, the red "CPU".
constexpr std::uint32_t kSetupStartAttach = 0x08AA86F0u;
constexpr std::uint32_t kSetupTexts = 0x08800B00u;   // the versus texts table, below the EBOOT
constexpr std::uint32_t kSetupLayout = 0x08800C00u;  // the versus row layout
constexpr std::uint32_t kSetupPlates = 0x08800D00u;  // the versus name plates

void put_entry(Guest *g, gaddr at, std::uint16_t part, std::uint16_t animation, std::uint16_t parent,
               std::uint16_t locator) {
    g_st32(g, at, part | static_cast<std::uint32_t>(animation) << 16u);
    g_st32(g, at + 4u, parent | static_cast<std::uint32_t>(locator) << 16u);
}

std::uint32_t setup_table(Guest *g, std::uint32_t table) {
    if (!versus::active()) return table;
    switch (table) {
    case 0x08AA83E0u: {
        static const std::uint16_t head[][4] = {{4, 5, 0, 9}, {5, 0, 4, 6}, {7, 6, 4, 6},
                                                {8, 8, 4, 5}, {9, 8, 4, 4}, {0xA, 8, 4, 3}};
        gaddr at = kSetupLayout;
        for (const auto &e : head) {
            put_entry(g, at, e[0], e[1], e[2], e[3]);
            at += 8u;
        }
        // Mode 2's from part 0xE on, with the terminator.
        for (gaddr p = 0x08AA85C0u; at < kSetupPlates; p += 8u, at += 8u) {
            g_st32(g, at, g_ld32(g, p));
            g_st32(g, at + 4u, g_ld32(g, p + 4u));
            if (ld16(g, p) == 0xFFFFu) break;
        }
        return kSetupLayout;
    }
    case kSetupStartAttach:
        put_entry(g, kSetupPlates, 0x3A, 0xA, 0, 7);
        put_entry(g, kSetupPlates + 8u, 0x3B, 0xC, 0, 6);
        put_entry(g, kSetupPlates + 16u, 0x3D, 1, 0x3B, 0);
        put_entry(g, kSetupPlates + 24u, 0xFFFF, 0xFFFF, 0xFFFF, 0);
        return kSetupPlates;
    case 0x08AA7EF8u: {
        // Quick Battle's first entry, then mode 2's from its third (its
        // first two are the name plates in its own style).
        g_st32(g, kSetupTexts, g_ld32(g, 0x08AA7EF8u));
        g_st32(g, kSetupTexts + 4u, g_ld32(g, 0x08AA7EFCu));
        std::uint32_t n = 1;
        for (gaddr p = 0x08AA8048u + 16u; n < 32u; p += 8u, ++n) {
            g_st32(g, kSetupTexts + 8u * n, g_ld32(g, p));
            g_st32(g, kSetupTexts + 8u * n + 4u, g_ld32(g, p + 4u));
            if (ld16(g, p + 4u) == 0xFFFFu) break;
        }
        return kSetupTexts;
    }
    default: return table;
    }
}

GuestRegs arguments(const decomp::CallState &in) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    return regs;
}

// main 0x088D3CF0 (layouts, parts, table): parts animated and attached.
void run_setup_attach(Guest *g, const decomp::CallState &in, GuestRegs &) {
    GuestRegs regs = arguments(in);
    regs.a[2] = setup_table(g, in.gpr[6]);
    decomp::call_original(g, 0x088D3CF0u, regs);
    if (in.gpr[6] == kSetupStartAttach && versus::active()) {
        // The setup's state is the parts' owner: parts at +0x10.
        const gaddr setup = in.gpr[5] - 0x10u;
        if (g_ld32(g, setup + 0x71Cu) == 0x08AA8790u) {
            g_st32(g, setup + 0x71Cu, 0x08AA87A8u);
            g_st8(g, setup + 0x720u, 4u);
            std::cout << "[versus] battle setup without the CPU rows" << std::endl;
        }
    }
}

// main 0x088D3F00 (layouts, parts, table): parts attached again.
void run_setup_reattach(Guest *g, const decomp::CallState &in, GuestRegs &) {
    GuestRegs regs = arguments(in);
    regs.a[2] = setup_table(g, in.gpr[6]);
    decomp::call_original(g, 0x088D3F00u, regs);
}

// main 0x088D44EC (parts, texts, table): texts placed on their parts.
void run_setup_place_texts(Guest *g, const decomp::CallState &in, GuestRegs &) {
    GuestRegs regs = arguments(in);
    regs.a[2] = setup_table(g, in.gpr[6]);
    decomp::call_original(g, 0x088D44ECu, regs);
}

struct Install {
    Install() {
        if (!versus::enabled()) return;
        using decomp::Module;
        using decomp::Returns;
        static const decomp::Override overrides[] = {
            {0x088D3B88u, "versus_menu_make_parts", Module::Main, Returns::Nothing, &run_make_parts},
            {0x088D428Cu, "versus_menu_attach_texts", Module::Main, Returns::Nothing, &run_attach_texts},
            {0x088D3C24u, "versus_menu_free_parts", Module::Main, Returns::Nothing, &run_free_parts},
            {0x088D3A04u, "versus_menu_free_layouts", Module::Main, Returns::Nothing, &run_free_layouts},
            {0x088D3CF0u, "versus_setup_attach", Module::Main, Returns::Nothing, &run_setup_attach},
            {0x088D3F00u, "versus_setup_reattach", Module::Main, Returns::Nothing, &run_setup_reattach},
            {0x088D44ECu, "versus_setup_place_texts", Module::Main, Returns::Nothing, &run_setup_place_texts},
            {0x088D46E4u, "versus_menu_cursor", Module::Main, Returns::Nothing, &run_cursor},
            {0x0887B924u, "versus_menu_text_show", Module::Main, Returns::Nothing, &run_text_show},
            {0x0887B944u, "versus_menu_text_hide", Module::Main, Returns::Nothing, &run_text_hide},
            {0x088D4BF4u, "versus_menu_sound", Module::Main, Returns::Integer, &run_sound},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
        // Overrides that only trace, there only with their switch.
        const char *trace = std::getenv("D012_TRACE_TEXTS");
        if (trace != nullptr && *trace != '\0' && std::strcmp(trace, "0") != 0) {
            static const decomp::Override traces[] = {
                {0x0887B900u, "versus_text_string", Module::Main, Returns::Nothing, &run_text_string},
                {0x0887AD78u, "versus_message_text", Module::Main, Returns::Integer, &run_message_text},
            };
            decomp::add_overrides(traces, sizeof traces / sizeof traces[0], true);
        }
    }
} install;

} // namespace
} // namespace d012::versus_menu
