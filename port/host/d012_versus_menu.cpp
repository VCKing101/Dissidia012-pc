// Local versus: the Battle Mode rows "Player vs Player (1-on-1)" and "Player
// vs Player (Party)" (docs/port/local-versus.md, sections 4, 7.6 and 0.2).
//
// The start menu (menu overlay) builds Battle Mode from tables in the
// overlay's data, through the submenu table at menu 0x089E7530 (0x18 bytes
// a submenu): +0x0C its parts (a layout instance each, an animation, the
// parent part and the parent's slot), +0x10 the texts on them, +0x14 its rows
// (two parts, the hand pointer's slot and a row type). Every part is an
// instance of the menu's layout 9, and every text a text object. Each row is
// one more row of the same kind: two parts made and attached as the game
// makes the others, texts made from strings and put on them, and the parts
// and rows tables copied below the EBOOT with the rows added and the submenu
// pointed at them.
//
// Both rows' type is 19, whose entry in the row-type table (menu 0x089E7488,
// 8 bytes a type) is the start of the submenu table: not enabled, help text
// 0. Battle Mode's own handler (menu 0x089E4A60) therefore only plays the
// "not allowed" sound for them, and the port takes it from there, by the
// cursor's row. Their help is shown and hidden through text 0, which no
// other row uses as a help: the help line's swap shows the new row's help
// and then hides the old row's, so a show of text 0 is the help of the row
// the cursor is on, and a hide of it is the help of the other versus row.
// (The game's rows types beyond 19 are its own, enabled or not by the save.)
//
// The menu's frame has five row slots (4 to 0). With the game's three rows the
// two versus rows take slots 1 and 0; with four (Labyrinth) the 1-on-1 row
// takes slot 0 and the Party row is put one row below it, its place carried
// on from slots 1 and 0, as is the hand pointer while it is on that row.
//
// Every hook here is a main function the menu calls, told apart by its
// caller. They were made when the harness took overrides in the main module
// and the battle overlay only; it takes the menu overlay's too now (player
// 2's pick in d012_versus.cpp), but a function called from its own 16 KiB
// unit still runs the original:
//
//   - main 0x088D3B88 (make parts from a table), at the menu's start (menu
//     0x089E4328): the submenu tables go back to the game's;
//   - main 0x088D428C (put texts on parts by a table), at the same start with
//     Battle Mode's text table: the rows' parts and texts are made;
//   - main 0x088D3C24 (release parts by a table), at the menu's end (menu
//     0x089E17F8): the rows' parts and texts are released;
//   - main 0x088D46E4 (a cursor), when Battle Mode opens (menu 0x089E45E8)
//     and when the start menu comes back in it after a destination (menu
//     0x089E5A04): two more rows, and the submenu pointed at the rows' tables;
//   - main 0x0887B924 / 0x0887B944 (show / hide a text), from the help line's
//     swap (menu 0x089E2A24): a row's help, and its highlight; and with the
//     1-on-1 row's label texts, the Party row's, which the menu's own texts
//     array has no room for;
//   - main 0x088D3AE8 (attach a part to a parent's slot), from the hand
//     pointer's placement (menu 0x089E2AEC): below slot 0 on the Party row
//     when there are four game rows;
//   - main 0x088D4BF4 (a menu sound), from Battle Mode's handler: a row
//     chosen; Quick Battle (1-on-1) or Quick Battle (Party) with versus on, or
//     "Connect a second controller";
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
constexpr std::uint32_t kQuickBattleParty = 3u;  // and of Quick Battle (Party)
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
// The rows' type (not enabled; its help text, when not enabled, 0).
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
// The rows' parts, in ids the start menu leaves free (its parts end at 70).
// The 1-on-1 row's label on its label part and the label's copy on its bar
// part (locator 1: the label as it reads on the lit bar) are menu texts 0x50
// and 0x51, shown and hidden with the submenu through its texts table as the
// other rows' are (the menu's own run to 0x4F, 0x52 up are made as the menu
// goes, and it releases 0 to 0x58 at its end). The game's rows have a third
// copy (bar, locator 0) for which there is no free id. The Party row's two
// label texts have no free id either: they are kept apart, in words of their
// own below the EBOOT, and shown and hidden with the 1-on-1 row's. Both rows'
// help is kept apart too, and put on the help line through those words.
constexpr std::uint32_t kPartLabel = 71u, kPartBar = 72u;
constexpr std::uint32_t kPartyPartLabel = 73u, kPartyPartBar = 74u;
constexpr std::uint32_t kTextLabel = 0x50u, kTextBarLit = 0x51u;
constexpr std::uint32_t kRowPrivate = 0x08800A80u;
constexpr std::uint32_t kPrivateHelp = 0u, kPrivatePartyLabel = 1u, kPrivatePartyBarLit = 2u, kPrivatePartyHelp = 3u;
constexpr std::uint32_t kPrivateCount = 4u;
constexpr std::uint32_t kPartFree = 0x088DA060u;  // (part)
constexpr std::uint32_t kLayoutFree = 0x088DAB44u;  // (layout), with its parts
constexpr std::uint32_t kTextFree = 0x0887BA5Cu;  // (text)
constexpr std::uint32_t kHelpPart = 0x32u;  // the help line, where every row's help goes (menu 0x089E6B66)
constexpr std::uint32_t kHelpFlags = 0x12u;
constexpr std::uint32_t kHandPart = 0x31u;  // the hand pointer (menu 0x089E2AEC)
constexpr std::uint32_t kHandPlaced = 0x089E2B24u;  // where 0x089E2AEC's attach returns
constexpr std::uint32_t kHandFollows = 0x089E1EB4u;  // and the frame's re-layout's (menu 0x089E1E5C)
constexpr std::uint32_t kLocator = 0x088DA0A8u;  // (part, slot, &matrix 0x40, &colour)
constexpr std::uint32_t kPartMatrix = 0x088D9B84u;  // (part, &matrix)
constexpr std::uint32_t kPartColour = 0x088D9AF4u;  // (part, &colour)
// Below the EBOOT (the split view's task and cursor are lower down).
constexpr std::uint32_t kRowParts = 0x08800800u;
constexpr std::uint32_t kRowTexts = 0x08800880u;
constexpr std::uint32_t kRowRows = 0x08800920u;
constexpr std::uint32_t kLabelString = 0x08800960u;
constexpr std::uint32_t kHelpString = 0x088009C0u;
constexpr std::uint32_t kPartyLabelString = 0x08801000u;
constexpr std::uint32_t kPartyHelpString = 0x08801080u;

const char kLabel[] = "Player vs Player (1-on-1)";
const char kHelp[] = "Fight a friend on a second controller, on a split screen.";
const char kPartyLabel[] = "Player vs Player (Party)";
const char kPartyHelp[] = "Party battles against a friend on a second controller, on a split screen.";

struct State {
    gaddr menu = 0;   // the start menu's state the rows were made for
    bool four = false;  // Battle Mode has four rows of its own
    std::uint32_t row = 0;  // the 1-on-1 row's index; the Party row's is one more
};

State &state() {
    static State s;
    return s;
}

std::uint32_t ld16(Guest *g, gaddr a) { return static_cast<std::uint16_t>(g_ld16s(g, a)); }
float ldf(Guest *g, gaddr a) {
    float v;
    std::memcpy(&v, g_ptr(g, a, 4u), 4u);
    return v;
}

bool from(const decomp::CallState &in, std::uint32_t begin, std::uint32_t end) {
    const std::uint32_t ra = in.gpr[31];
    return ra >= begin && ra < end;
}

GuestRegs arguments_of(const decomp::CallState &in) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    return regs;
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

// A part attached one row below the frame's slot 0, as main 0x088D3AE8
// attaches to a slot: the parent's locator matrix and colour handed to the
// part, the matrix carried on from slots 1 and 0 (slot 0's, plus the step
// from slot 1 to slot 0).
void attach_below_slot0(Guest *g, gaddr parts, std::uint32_t parent, std::uint32_t child) {
    const gaddr frame = g_ld32(g, parts + 4u * parent), part = g_ld32(g, parts + 4u * child);
    if (frame == 0u || part == 0u) return;
    const gaddr buffer = g_push(g, 0xA0u);
    const gaddr slot0 = buffer, colour = buffer + 0x40u, slot1 = buffer + 0x50u, spare = buffer + 0x90u;
    g_call4(g, kLocator, frame, 0u, slot0, colour);
    g_call4(g, kLocator, frame, 1u, slot1, spare);
    for (std::uint32_t i = 0; i < 16u; ++i) {
        const float a = ldf(g, slot0 + 4u * i), b = ldf(g, slot1 + 4u * i);
        g_stf(g, slot0 + 4u * i, a + (a - b));
    }
    g_call2(g, kPartMatrix, part, slot0);
    g_call2(g, kPartColour, part, colour);
    g_pop(g, 0xA0u);
}

// The rows' parts and texts, made as the menu makes its own (main
// 0x088D3B88, then the attachments of menu 0x089E20AC, then main 0x088D3F7C
// and 0x088D428C), and the copies of Battle Mode's tables.
void make_row(Guest *g, gaddr menu) {
    State &s = state();
    const gaddr parts = menu + kStateParts;
    s.four = g_ld32(g, kSubmenuParts) == kGameParts4;
    s.row = s.four ? 4u : 3u;
    // The slots under the game's last row: 1 and 0 with three rows; with
    // four, 0 and the place below it (attach_below_slot0).
    const std::uint32_t slot = s.four ? 0u : 1u;
    const std::uint32_t party_slot = 0u;
    const gaddr layout = g_ld32(g, menu + kStateLayouts + 9u * 4u);
    const std::uint32_t looks[4][3] = {{kPartLabel, 0x12u, 0u}, {kPartBar, 0x11u, 0u}, {kPartyPartLabel, 0x12u, 1u},
                                       {kPartyPartBar, 0x11u, 1u}};
    for (const auto &[id, look, party] : looks) {
        const gaddr part = g_call1(g, kPartClone, layout);
        g_st32(g, parts + 4u * id, part);
        if (part == 0u) return;
        g_call3(g, kPartAnimate, part, look, 1u);
        if (party != 0u && s.four)
            attach_below_slot0(g, parts, kParent, id);
        else
            g_call4(g, kPartAttach, parts, kParent, party != 0u ? party_slot : slot, id);
        g_call1(g, kPartHide, part);
        g_st32(g, part + 0x30u, 0u);
        g_st32(g, part + 0x34u, 0u);
        g_st32(g, part + 0x38u, 0u);
    }
    write_utf16(g, kLabelString, kLabel);
    write_utf16(g, kHelpString, kHelp);
    write_utf16(g, kPartyLabelString, kPartyLabel);
    write_utf16(g, kPartyHelpString, kPartyHelp);
    const gaddr texts = menu + kStateTexts;
    // A label and its copy for the lit bar.
    const auto label_pair = [&](gaddr string, gaddr where, std::uint32_t label_id, std::uint32_t lit_id) {
        const std::uint32_t label = text_from_string(g, string);
        g_st32(g, where + 4u * label_id, label);
        const std::uint32_t copy = label != 0u ? g_call1(g, kTextCopy, label) : 0u;
        if (copy != 0u) {
            g_call1(g, kTextSetUp, copy);
            g_st32(g, copy + 0x10u, g_ld32(g, copy + 0x10u) & ~0x400u);
        }
        g_st32(g, where + 4u * lit_id, copy);
    };
    label_pair(kLabelString, texts, kTextLabel, kTextBarLit);
    label_pair(kPartyLabelString, kRowPrivate, kPrivatePartyLabel, kPrivatePartyBarLit);
    const std::uint32_t on_parts[4][5] = {{texts, kTextLabel, 9u, kPartLabel, 0u},
                                          {texts, kTextBarLit, 9u, kPartBar, 1u},
                                          {kRowPrivate, kPrivatePartyLabel, 9u, kPartyPartLabel, 0u},
                                          {kRowPrivate, kPrivatePartyBarLit, 9u, kPartyPartBar, 1u}};
    for (const auto &e : on_parts) {
        const gaddr text = g_ld32(g, e[0] + 4u * e[1]);
        if (text == 0u) continue;
        g_call8(g, kTextAttach, parts, e[0], e[1], e[2], e[3], e[4], 0u, 0u);
        // Attaching shows a text; the submenu shows the menu's own.
        g_st32(g, text + 0x10u, g_ld32(g, text + 0x10u) & ~0x400u);
    }
    // Putting a text on a part shows it; the help line shows a row's help
    // only while the row is chosen.
    const std::uint32_t helps[2][2] = {{kHelpString, kPrivateHelp}, {kPartyHelpString, kPrivatePartyHelp}};
    for (const auto &[string, id] : helps) {
        const std::uint32_t help = text_from_string(g, string);
        g_st32(g, kRowPrivate + 4u * id, help);
        if (help == 0u) continue;
        g_call8(g, kTextAttach, parts, kRowPrivate, id, kHelpFlags, kHelpPart, 0u, 0u, 0u);
        g_st32(g, help + 0x10u, g_ld32(g, help + 0x10u) & ~0x400u);
    }

    const auto u16 = [](std::uint32_t v) { return static_cast<std::uint16_t>(v); };
    const std::uint16_t row_parts[4][4] = {{u16(kPartLabel), 0x12u, u16(kParent), u16(slot)},
                                           {u16(kPartBar), 0x11u, u16(kParent), u16(slot)},
                                           {u16(kPartyPartLabel), 0x12u, u16(kParent), u16(party_slot)},
                                           {u16(kPartyPartBar), 0x11u, u16(kParent), u16(party_slot)}};
    copy_table(g, kRowParts, g_ld32(g, kSubmenuParts), [&](gaddr p) { return ld16(g, p) == 0xFFFFu; }, row_parts, 4u);
    const std::uint16_t row_texts[2][4] = {{u16(kTextLabel), 9u, u16(kPartLabel), 0u}, {u16(kTextBarLit), 9u, u16(kPartBar), 1u}};
    copy_table(g, kRowTexts, kGameTexts, [&](gaddr p) { return ld16(g, p + 4u) == 0xFFFFu; }, row_texts, 2u);
    const std::uint16_t rows[2][4] = {{u16(kPartLabel), u16(kPartBar), u16(slot), u16(kRowType)},
                                      {u16(kPartyPartLabel), u16(kPartyPartBar), u16(party_slot), u16(kRowType)}};
    copy_table(g, kRowRows, kGameRows, [&](gaddr p) { return ld16(g, p + 6u) == 0xFFFFu; }, rows, 2u, s.row);
    s.menu = menu;
    std::cout << "[versus] Battle Mode rows made (" << (s.four ? "after four rows" : "after three rows") << "), texts 0x"
              << std::hex << g_ld32(g, texts + 4u * kTextLabel) << " 0x" << g_ld32(g, kRowPrivate + 4u * kPrivatePartyLabel)
              << " 0x" << g_ld32(g, kRowPrivate + 4u * kPrivateHelp) << " 0x"
              << g_ld32(g, kRowPrivate + 4u * kPrivatePartyHelp) << std::dec << std::endl;
}

// Battle Mode on the rows' tables (while the start menu lasts).
void point_at_row(Guest *g) {
    g_st32(g, kSubmenuParts, kRowParts);
    g_st32(g, kSubmenuTexts, kRowTexts);
    g_st32(g, kSubmenuRows, kRowRows);
}

// The rows' parts released, as the menu releases its own when it ends (menu
// 0x089E17F8); the 1-on-1 row's label texts go with the menu's texts
// afterwards.
void free_row_parts(Guest *g, gaddr parts) {
    for (const std::uint32_t id : {kPartLabel, kPartBar, kPartyPartLabel, kPartyPartBar}) {
        const gaddr part = g_ld32(g, parts + 4u * id);
        if (part != 0u) g_call1(g, kPartFree, part);
        g_st32(g, parts + 4u * id, 0u);
    }
}

// The rows' own texts released, after every part (their parts listed them).
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

// As the start menu begins (menu 0x089E4328, after the row is made), menu
// 0x089E5A04 brings back the submenu it was left from (state +0xA2C), its
// cursor set here before 0x089E23C8 shows it: after any Battle Mode
// destination, the menu comes back in Battle Mode without passing its main
// row.
constexpr std::uint32_t kSubmenuBack = 0x089E5A04u, kSubmenuBackEnd = 0x089E5C18u;

void run_cursor(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr cursor = in.gpr[4];
    std::int32_t last = static_cast<std::int32_t>(in.gpr[7]);
    if (last < 0) last = 0;
    const State &s = state();
    if (s.menu != 0u && cursor == s.menu + kStateCursor &&
        (from(in, kMainRows, kMainRowsEnd) || from(in, kSubmenuBack, kSubmenuBackEnd)) && menu_resident(g) &&
        g_ld32(g, s.menu + kStateMainRow) == kBattleModeSubmenu) {
        last = static_cast<std::int32_t>(s.row + 1u);
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

// A row's highlight, as menu 0x089E56F8 lights a row and puts it out.
void light_row(Guest *g, std::uint32_t bar_id, bool lit) {
    const gaddr bar = g_ld32(g, state().menu + kStateParts + 4u * bar_id);
    if (bar == 0u) return;
    if (lit) {
        g_call3(g, kPartAnimate, bar, 0xFu, 0u);
        g_st16(g, bar + 0xEu, 0x10u);
    } else {
        g_call3(g, kPartAnimate, bar, 0x11u, 0u);
    }
}

// The help line's swap (menu 0x089E2A24: the new row's help shown, then the
// old row's hidden) shows and hides text 0 for the versus rows: their help
// instead, and their bar's highlight. A show is for the row the cursor is on.
// A hide is for the row the cursor was on: every versus row but the one this
// same swap has just shown (the cursor may still name the old row, as when
// Battle Mode is left for the main menu's rows).
// Whether the swap's text was one of the versus rows'.
bool swap_row_help(Guest *g, const decomp::CallState &in, gaddr text, bool shown) {
    const State &s = state();
    static const bool trace = std::getenv("D012_TRACE_VERSUS_MENU") != nullptr;
    // The row the swap's show lit, for its hide (-1: it showed another row's).
    static std::int64_t swap_shown = -1;
    if (s.menu == 0u || !menu_resident(g) || !from(in, kHelpSwap, kHelpSwapEnd)) return false;
    const bool ours = text == g_ld32(g, s.menu + kStateTexts);
    const std::uint32_t row = g_ld32(g, s.menu + kStateRow);
    if (shown) swap_shown = ours && in_battle_mode(g) ? static_cast<std::int64_t>(row) : -1;
    if (!ours) return false;
    const struct {
        std::uint32_t row, help, bar;
    } rows[2] = {{s.row, kPrivateHelp, kPartBar}, {s.row + 1u, kPrivatePartyHelp, kPartyPartBar}};
    for (const auto &r : rows) {
        if (shown ? swap_shown != static_cast<std::int64_t>(r.row) : swap_shown == static_cast<std::int64_t>(r.row)) continue;
        const gaddr help = g_ld32(g, kRowPrivate + 4u * r.help);
        if (help != 0u) {
            const std::uint32_t flags = g_ld32(g, help + 0x10u);
            g_st32(g, help + 0x10u, shown ? flags | 0x400u : flags & ~0x400u);
        }
        light_row(g, r.bar, shown);
        if (trace) std::printf("[versus-menu] row %u's help %s (cursor on row %u)\n", r.row, shown ? "shown" : "hidden", row);
    }
    if (!shown) swap_shown = -1;
    return true;
}

// The 1-on-1 row's label texts shown or hidden (with the submenu): the Party
// row's with them.
void party_labels_follow(Guest *g, gaddr text, bool shown) {
    const State &s = state();
    if (s.menu == 0u || !menu_resident(g) || text == 0u) return;
    const gaddr texts = s.menu + kStateTexts;
    std::uint32_t id = 0xFFFFFFFFu;
    if (text == g_ld32(g, texts + 4u * kTextLabel)) id = kPrivatePartyLabel;
    if (text == g_ld32(g, texts + 4u * kTextBarLit)) id = kPrivatePartyBarLit;
    if (id == 0xFFFFFFFFu) return;
    const gaddr party = g_ld32(g, kRowPrivate + 4u * id);
    if (party == 0u) return;
    const std::uint32_t flags = g_ld32(g, party + 0x10u);
    g_st32(g, party + 0x10u, shown ? flags | 0x400u : flags & ~0x400u);
    static const bool trace = std::getenv("D012_TRACE_VERSUS_MENU") != nullptr;
    if (trace)
        std::printf("[versus-menu] party label %u %s (text 0x%08X flags 0x%08X -> 0x%08X)\n", id, shown ? "shown" : "hidden",
                    party, flags, g_ld32(g, party + 0x10u));
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
    if (!on || !(versus::split_recently() || versus::party()) || told >= 4000u) return;
    ++told;
    std::printf("[text] %s %08X from %08X\n", what, in.gpr[4], in.gpr[31]);
}

void run_text_show(Guest *g, const decomp::CallState &in, GuestRegs &) {
    trace_text("show", in);
    const gaddr text = in.gpr[4];
    if (swap_row_help(g, in, text, true)) return;
    g_st32(g, text + 0x10u, g_ld32(g, text + 0x10u) | 0x400u);
    party_labels_follow(g, text, true);
}

// main 0x0887B900 (text, id): the text's string (+8), as the game does it; for
// D012_TRACE_TEXTS, logged with the caller.
void run_text_string(Guest *g, const decomp::CallState &in, GuestRegs &) {
    static const bool on = [] {
        const char *text = std::getenv("D012_TRACE_TEXTS");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    static std::uint32_t told = 0;
    if (on && (versus::split_recently() || versus::party()) && told < 4000u) {
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
    if (on && (versus::split_recently() || versus::party()) && told < 4000u) {
        ++told;
        std::printf("[text] made %08X from message file %08X, %08X, message %u, from %08X\n", regs.v0, in.gpr[4],
                    in.gpr[5], in.gpr[6], in.gpr[31]);
    }
}

// main 0x0887B944 (text): hidden.
void run_text_hide(Guest *g, const decomp::CallState &in, GuestRegs &) {
    trace_text("hide", in);
    const gaddr text = in.gpr[4];
    if (swap_row_help(g, in, text, false)) return;
    g_st32(g, text + 0x10u, g_ld32(g, text + 0x10u) & ~0x400u);
    party_labels_follow(g, text, false);
}

// main 0x088D3AE8 (parts, parent, slot, child): a part attached to its
// parent's slot. From the hand pointer's placement (menu 0x089E2AEC) on the
// Party row when Battle Mode has four rows of its own, below slot 0 instead;
// the same from the re-layout every frame (menu 0x089E1E5C), which attaches
// the hand again from the slot 0x089E2AEC kept.
void run_part_attach(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr parts = in.gpr[4];
    const std::uint32_t parent = in.gpr[5], slot = in.gpr[6], child = in.gpr[7];
    const State &s = state();
    if ((in.gpr[31] == kHandPlaced || in.gpr[31] == kHandFollows) && child == kHandPart && slot == 0u && s.four && in_battle_mode(g) &&
        parts == s.menu + kStateParts && g_ld32(g, s.menu + kStateRow) == s.row + 1u) {
        attach_below_slot0(g, parts, parent, child);
        return;
    }
    GuestRegs regs = arguments_of(in);
    decomp::call_original(g, kPartAttach, regs);
}

// main 0x088D4BF4 (sound) -> the voice, or 0.
void run_sound(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    std::uint32_t sound = in.gpr[4];
    static const bool trace = std::getenv("D012_TRACE_VERSUS_MENU") != nullptr;
    if (trace) std::printf("[versus-menu] menu sound %u from 0x%08X\n", sound, in.gpr[31]);
    const State &s = state();
    const std::uint32_t row = s.menu != 0u ? g_ld32(g, s.menu + kStateRow) : 0xFFFFFFFFu;
    if (sound == kSoundRefused && from(in, kBattleMode, kBattleModeEnd) && in_battle_mode(g) &&
        (row == s.row || row == s.row + 1u)) {
        const bool party = row == s.row + 1u;
        if (versus::second_pad_connected()) {
            // Quick Battle (1-on-1) or (Party), as their own rows leave for them.
            g_st32(g, kDestination, party ? kQuickBattleParty : kQuickBattle);
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

// D012_TRACE_PARTY=1: in Player vs Player (Party), the tables the menus hand
// the attach and place functions, with their callers.
void trace_table(const char *what, const decomp::CallState &in, std::uint32_t table) {
    static const bool on = [] {
        const char *text = std::getenv("D012_TRACE_PARTY");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    // Each table and caller once.
    static std::uint64_t seen[512];
    static std::uint32_t count = 0;
    if (!on || !versus::party() || count >= 512u) return;
    const std::uint64_t key = static_cast<std::uint64_t>(table) << 32 | in.gpr[31];
    for (std::uint32_t i = 0; i < count; ++i)
        if (seen[i] == key) return;
    seen[count++] = key;
    std::printf("[party-ui] %s table 0x%08X from 0x%08X\n", what, table, in.gpr[31]);
}

GuestRegs arguments(const decomp::CallState &in) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    return regs;
}

// The rows' parts table attached (main 0x088D3CF0 and 0x088D3F00): with four
// game rows, the Party row's parts below slot 0 again.
void party_row_parts_below(Guest *g, gaddr parts, gaddr table) {
    const State &s = state();
    if (table != kRowParts || !s.four || s.menu == 0u || parts != s.menu + kStateParts || !menu_resident(g)) return;
    attach_below_slot0(g, parts, kParent, kPartyPartLabel);
    attach_below_slot0(g, parts, kParent, kPartyPartBar);
}

// The rows' texts table placed (main 0x088D44EC): the Party row's labels,
// which the table cannot name, placed on their parts as well (main
// 0x088D4584 hands a text its part's locator, once).
constexpr std::uint32_t kPlaceText = 0x088D4584u;  // (parts, part, locator, text)

void place_party_labels(Guest *g, gaddr parts, gaddr table) {
    const State &s = state();
    if (table != kRowTexts || s.menu == 0u || parts != s.menu + kStateParts || !menu_resident(g)) return;
    const std::uint32_t labels[2][3] = {{kPrivatePartyLabel, kPartyPartLabel, 0u}, {kPrivatePartyBarLit, kPartyPartBar, 1u}};
    for (const auto &[id, part, locator] : labels) {
        const gaddr text = g_ld32(g, kRowPrivate + 4u * id);
        if (text != 0u && g_ld32(g, parts + 4u * part) != 0u) g_call4(g, kPlaceText, parts, part, locator, text);
    }
}

// ---- Player vs Player (Party): a fight's settings --------------------------
//
// Quick Battle (Party)'s setup opens a dialog for each fight (menu 0x08AA9634
// sets it up, on its work area *(object + 0x7CFC): parts at +0x10, the cursor
// at +0x5CC). It picks its layout by mode: 0x08AB0418 online (Stage, Rules,
// Judgment, the online flag +0x7CC0 bit 0, which some 30 places read),
// 0x08AB0508, and offline 0x08AB02B0: six row places on the dialog frame's
// locators 5 to 0, Stage, Strength (CPU), Level (CPU), Type (CPU), Rules and
// Judgment (row parts 85, 90, 95, 100, 105, 110, each with four parts of its
// own), of which the cursor runs over the first five. In versus the CPU rows
// mean nothing (player 2 plays, on player 1's saved builds), so each frame,
// as the dialog attaches its parts again (main 0x088D3F00 from menu
// 0x08AA9898), their parts are hidden and the cursor is given them as rows to
// pass over: the game's cursor (main 0x088D3558) steps past the rows listed
// at its +0x28 (a count at +0x1C), so up and down go from Stage to Rules and
// back, with the dialog's own highlight. Rules keeps its place, with a gap
// above it. A cursor that opens on one of them (a fight's last row) goes to
// Stage.
constexpr std::uint32_t kPartyDialogLayout = 0x08AB02B0u;
constexpr std::uint32_t kPartyDialogParts = 0x10u;    // in the dialog's work
constexpr std::uint32_t kPartyDialogCursor = 0x5CCu;  // +4 its row
constexpr std::uint32_t kCursorSkipCount = 0x1Cu, kCursorSkip = 0x28u;
constexpr std::uint32_t kPartyDialogSkip = 0x08801200u;  // below the EBOOT: rows 1, 2, 3
constexpr std::uint32_t kPartyDialogStage = 0u;

void party_dialog_without_cpu_rows(Guest *g, gaddr parts, gaddr table) {
    if (table != kPartyDialogLayout || !versus::party()) return;
    static const std::uint32_t cpu_parts[] = {90u, 91u, 92u, 93u, 94u, 95u, 96u, 97u, 98u, 99u, 100u, 101u, 102u, 103u, 104u};
    for (const std::uint32_t id : cpu_parts) {
        const gaddr part = g_ld32(g, parts + 4u * id);
        if (part != 0u) g_call1(g, kPartHide, part);
    }
    const gaddr cursor = parts - kPartyDialogParts + kPartyDialogCursor;
    const gaddr skip = g_ld32(g, cursor + kCursorSkip);
    if (skip == 0u || skip == kPartyDialogSkip) {
        for (std::uint32_t i = 0; i < 3u; ++i) g_st32(g, kPartyDialogSkip + 4u * i, i + 1u);
        g_st32(g, cursor + kCursorSkip, kPartyDialogSkip);
        g_st32(g, cursor + kCursorSkipCount, 3u);
    } else {
        static bool told = false;
        if (!told) {
            told = true;
            std::cout << "[versus] a fight's settings dialog with rows of its own to pass over: its CPU rows left in reach"
                      << std::endl;
        }
    }
    const std::uint32_t row = g_ld32(g, cursor + 4u);
    if (row >= 1u && row <= 3u) g_st32(g, cursor + 4u, kPartyDialogStage);
}

// main 0x088D3CF0 (layouts, parts, table): parts animated and attached.
void run_setup_attach(Guest *g, const decomp::CallState &in, GuestRegs &) {
    trace_table("attach", in, in.gpr[6]);
    GuestRegs regs = arguments(in);
    regs.a[2] = setup_table(g, in.gpr[6]);
    decomp::call_original(g, 0x088D3CF0u, regs);
    party_row_parts_below(g, in.gpr[5], in.gpr[6]);
    party_dialog_without_cpu_rows(g, in.gpr[5], in.gpr[6]);
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
    trace_table("attach again", in, in.gpr[6]);
    GuestRegs regs = arguments(in);
    regs.a[2] = setup_table(g, in.gpr[6]);
    decomp::call_original(g, 0x088D3F00u, regs);
    party_row_parts_below(g, in.gpr[5], in.gpr[6]);
    party_dialog_without_cpu_rows(g, in.gpr[5], in.gpr[6]);
}

// main 0x088D44EC (parts, texts, table): texts placed on their parts.
void run_setup_place_texts(Guest *g, const decomp::CallState &in, GuestRegs &) {
    trace_table("place texts", in, in.gpr[6]);
    GuestRegs regs = arguments(in);
    regs.a[2] = setup_table(g, in.gpr[6]);
    decomp::call_original(g, 0x088D44ECu, regs);
    place_party_labels(g, in.gpr[4], in.gpr[6]);
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
            {kPartAttach, "versus_menu_part_attach", Module::Main, Returns::Nothing, &run_part_attach},
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
