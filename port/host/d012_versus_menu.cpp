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
//     controller".
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
// Its label and the label's copy on the bar part are menu texts 0x50 and
// 0x51, shown and hidden with the submenu through its texts table as the
// other rows' are (the menu's own run to 0x4F, and 0x52 up are made as the
// menu goes; it releases 0 to 0x58 at its end). The game's rows have a second
// copy on the bar part (locator 1); the row has room for one. The help is
// kept apart, in a word of its own below the EBOOT, and put on the help line
// through it.
constexpr std::uint32_t kPartLabel = 71u, kPartBar = 72u;
constexpr std::uint32_t kTextLabel = 0x50u, kTextBar = 0x51u;
constexpr std::uint32_t kRowHelp = 0x08800A80u;
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
    std::uint32_t copy = label != 0u ? g_call1(g, kTextCopy, label) : 0u;
    if (copy != 0u) {
        g_call1(g, kTextSetUp, copy);
        g_st32(g, copy + 0x10u, g_ld32(g, copy + 0x10u) & ~0x400u);
    }
    g_st32(g, texts + 4u * kTextBar, copy);
    const std::uint32_t on_parts[2][4] = {{kTextLabel, 9u, kPartLabel, 0u}, {kTextBar, 9u, kPartBar, 0u}};
    for (const auto &e : on_parts)
        if (g_ld32(g, texts + 4u * e[0]) != 0u) g_call8(g, kTextAttach, parts, texts, e[0], e[1], e[2], e[3], 0u, 0u);
    const std::uint32_t help = text_from_string(g, kHelpString);
    g_st32(g, kRowHelp, help);
    // Putting a text on a part shows it; the help line shows the row's help
    // only while the row is chosen.
    if (help != 0u) {
        g_call8(g, kTextAttach, parts, kRowHelp, 0u, kHelpFlags, kHelpPart, 0u, 0u, 0u);
        g_st32(g, help + 0x10u, g_ld32(g, help + 0x10u) & ~0x400u);
    }

    const std::uint16_t row_parts[2][4] = {{static_cast<std::uint16_t>(kPartLabel), 0x12u, static_cast<std::uint16_t>(kParent),
                                            static_cast<std::uint16_t>(slot)},
                                           {static_cast<std::uint16_t>(kPartBar), 0x11u, static_cast<std::uint16_t>(kParent),
                                            static_cast<std::uint16_t>(slot)}};
    copy_table(g, kRowParts, g_ld32(g, kSubmenuParts), [&](gaddr p) { return ld16(g, p) == 0xFFFFu; }, row_parts, 2u);
    const std::uint16_t row_texts[2][4] = {{static_cast<std::uint16_t>(kTextLabel), 9u, static_cast<std::uint16_t>(kPartLabel), 0u},
                                           {static_cast<std::uint16_t>(kTextBar), 9u, static_cast<std::uint16_t>(kPartBar), 0u}};
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

// The row's help released, after every part (the help line's part listed it).
void free_row_help(Guest *g) {
    const gaddr help = g_ld32(g, kRowHelp);
    if (help != 0u) g_call1(g, kTextFree, help);
    g_st32(g, kRowHelp, 0u);
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
void run_attach_texts(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr parts = in.gpr[4], texts = in.gpr[5], table = in.gpr[6];
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
    if (state().menu != 0u && layouts == state().menu + kStateLayouts && menu_resident(g)) free_row_help(g);
}

// main 0x088D46E4 (cursor, row, a2, last row, t0, t1).
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
                    g_ld32(g, kRowHelp));
    if (!from(in, kHelpSwap, kHelpSwapEnd) || !in_battle_mode(g)) return 0u;
    if (text != g_ld32(g, state().menu + kStateTexts)) return 0u;
    return g_ld32(g, kRowHelp);
}

// main 0x0887B924 (text): shown.
void run_text_show(Guest *g, const decomp::CallState &in, GuestRegs &) {
    gaddr text = in.gpr[4];
    if (const gaddr help = help_for_row(g, in, text); help != 0u) {
        text = help;
        light_row(g, true);
    }
    g_st32(g, text + 0x10u, g_ld32(g, text + 0x10u) | 0x400u);
}

// main 0x0887B944 (text): hidden.
void run_text_hide(Guest *g, const decomp::CallState &in, GuestRegs &) {
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
            {0x088D46E4u, "versus_menu_cursor", Module::Main, Returns::Nothing, &run_cursor},
            {0x0887B924u, "versus_menu_text_show", Module::Main, Returns::Nothing, &run_text_show},
            {0x0887B944u, "versus_menu_text_hide", Module::Main, Returns::Nothing, &run_text_hide},
            {0x088D4BF4u, "versus_menu_sound", Module::Main, Returns::Integer, &run_sound},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
    }
} install;

} // namespace
} // namespace d012::versus_menu
