// Who is where, on the game's own Online Lobby screen: the room list shows how
// many players are in each room, and the title bar how many are online, in
// battles and in Group Battle, from the server the game is online with
// (d012_online.hpp's where(), d012_where.hpp).
//
// The room list (menu overlay, the Online Lobby's object; its work area at
// object +0x1320: layouts at +0, parts at +0x400, texts at +0x880):
//   - is set up by menu 0x08A0C3C4: parts from table 0x08A11528, attached by
//     0x08A11708, texts made from messages by 0x08A11838 and put on parts by
//     0x08A11916;
//   - is drawn each frame by menu 0x08A0C560: main 0x088D3F00 attaches the
//     parts again, then main 0x088D44EC places the texts of 0x08A11916 (the
//     hook, returning to 0x08A0C59C);
//   - rooms 1 to 16 are the bars, parts 48 to 63 (frame part 45's slots 17
//     to 2), each with its name, texts 27 to 42, on its locator 0, left
//     aligned; part 46 is the left column's header;
//   - its object is released by menu 0x08A0DD88: the layouts (main
//     0x088D3A04), then texts 0 to 97 (main 0x0887BA5C).
// No table of the object's names texts 0 to 26, and none of its code makes
// them, so the counts are kept there: texts 0 to 15 for the rooms, 16 for the
// title bar, made from strings with the game's font, put on their parts as
// the game puts its own (main 0x088D40BC), placed with them each frame (main
// 0x088D4584) and moved to the bar's right end. A count that changes is a new
// text: the old one taken off its part's list of texts (part +0x30 first,
// +0x34 last, +0x38 how many; a text's +0x4C next, +0x54 the one before) and
// released. The game releases the last ones with its own.
//
// Each time the hand moves, menu 0x08A0BEF8 empties every bar's list of texts
// and puts the names back (the lit copy, texts 43 to 58, on the chosen bar), so
// a count no longer on its bar's list is put back on it.
//
// D012_NO_LOBBY_STATUS=1 turns it off.
// MIT licence; see port/README.md.

#include "d012_lobby_status.hpp"

#include "d012_battle_relay.hpp"
#include "d012_online.hpp"
#include "d012_versus.hpp"

#include "adhoc/session.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

namespace d012::lobby_status {
namespace {

// main
constexpr std::uint32_t kPlaceTexts = 0x088D44ECu;  // (parts, texts, table)
constexpr std::uint32_t kPlaceText = 0x088D4584u;   // (parts, part, locator, text)
constexpr std::uint32_t kTextAttach = 0x088D40BCu;  // (parts, texts, text, flags, part, locator)
constexpr std::uint32_t kTextFromString = 0x0887D868u;  // (font, 0, utf16, -1, 0, 0, 0, 0)
constexpr std::uint32_t kTextSetUp = 0x0887BA04u;
constexpr std::uint32_t kTextFree = 0x0887BA5Cu;
constexpr std::uint32_t kFont = 0x089D6998u;  // -> the text font
// menu overlay: the room list's per-frame placing of its texts
constexpr std::uint32_t kRoomListPlaced = 0x08A0C59Cu;  // the return from its call
constexpr std::uint32_t kRoomListTexts = 0x08A11916u;
constexpr std::uint32_t kRoomListCall = 0x0E23513Bu;     // at 0x08A0C594: jal 0x088D44EC
constexpr std::uint32_t kRoomListTable = 0x24C61916u;    // at 0x08A0C598: addiu a2, a2, 0x1916
// The room list's parts and our texts.
constexpr std::uint32_t kFirstBar = 48u;    // room 1's bar; room n's is 47 + n
constexpr std::uint32_t kHeaderPart = 46u;  // the left column's header
constexpr std::uint32_t kRooms = 16u;
constexpr std::uint32_t kFirstLitCopy = 43u;  // room 1's name, lit, on the chosen bar
constexpr std::uint32_t kSummaryText = 16u;  // texts 0 to 15 the rooms'
constexpr std::uint32_t kOurTexts = 17u;
// Right and middle (main 0x088D40BC: bit 2 right, bit 4 middle).
constexpr std::uint32_t kRightMiddle = 0x14u;
// From a room's name (locator 0) to the count's right edge: the bar ends
// 145 pixels on (PSP pixels, measured on both columns).
constexpr int kCountRight = 139;
// The title bar's right end, as the game's button hints sit, and their ink.
constexpr int kSummaryX = 466, kSummaryY = 16;
constexpr std::uint8_t kHintInk[3] = {39u, 41u, 65u};
// Below the EBOOT: the string a text is made from.
constexpr std::uint32_t kString = 0x08802000u;
constexpr std::size_t kStringRoom = 0x100u;

bool off() {
    static const bool value = [] {
        const char *text = std::getenv("D012_NO_LOBBY_STATUS");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    return value;
}

// The texts this module made on one room list.
struct Shown {
    gaddr texts = 0u;                         // the room list's texts array
    std::array<gaddr, kOurTexts> made{};      // our text objects
    std::array<std::string, kOurTexts> says;  // what each says ("" none)
};

Shown &shown() {
    static Shown s;
    return s;
}

std::uint32_t part_of(std::uint32_t id) { return id < kRooms ? kFirstBar + id : kHeaderPart; }

std::string players(unsigned count) {
    return std::to_string(count) + (count == 1u ? " player" : " players");
}

// What each of our texts should say.
std::array<std::string, kOurTexts> wanted(const net::relay::Where &where) {
    std::array<std::string, kOurTexts> says;
    for (std::uint32_t room = 0; room < kRooms; ++room)
        if (where.lobbies[room] != 0u) says[room] = players(where.lobbies[room]);
    std::string summary = std::to_string(where.online) + " online";
    if (where.duels != 0u) summary += ", " + std::to_string(where.duels) + " in battles";
    const unsigned group = static_cast<unsigned>(where.group_battle) + where.party_battles;
    if (group != 0u) summary += ", " + std::to_string(group) + " in Group Battle";
    if (where.communications != 0u) summary += ", " + std::to_string(where.communications) + " in Communications";
    says[kSummaryText] = summary;
    return says;
}

// Whether `text` is on the list of texts of part `part_id`.
bool listed(Guest *g, gaddr parts, std::uint32_t part_id, gaddr text) {
    const gaddr part = g_ld32(g, parts + 4u * part_id);
    if (part == 0u) return false;
    for (gaddr t = g_ld32(g, part + 0x30u), n = 0u; t != 0u && n < 256u; t = g_ld32(g, t + 0x4Cu), ++n)
        if (t == text) return true;
    return false;
}

// Off its part's list of texts, if it is on it, and released.
void release(Guest *g, gaddr parts, std::uint32_t part_id, gaddr text) {
    const gaddr part = g_ld32(g, parts + 4u * part_id);
    if (listed(g, parts, part_id, text)) {
        const gaddr next = g_ld32(g, text + 0x4Cu), before = g_ld32(g, text + 0x54u);
        if (before != 0u) g_st32(g, before + 0x4Cu, next);
        else g_st32(g, part + 0x30u, next);
        if (next != 0u) g_st32(g, next + 0x54u, before);
        else g_st32(g, part + 0x34u, before);
        const std::uint32_t count = g_ld32(g, part + 0x38u);
        if (count != 0u) g_st32(g, part + 0x38u, count - 1u);
        g_st32(g, text + 0x4Cu, 0u);
        g_st32(g, text + 0x54u, 0u);
    }
    g_call1(g, kTextFree, text);
}

gaddr text_from(Guest *g, const std::string &says) {
    const std::size_t length = std::min(says.size(), kStringRoom / 2u - 1u);
    for (std::size_t i = 0; i < length; ++i)
        g_st16(g, kString + 2u * static_cast<std::uint32_t>(i), static_cast<std::uint8_t>(says[i]));
    g_st16(g, kString + 2u * static_cast<std::uint32_t>(length), 0u);
    const gaddr text = g_call8(g, kTextFromString, g_ld32(g, kFont), 0u, kString, 0xFFFFFFFFu, 0u, 0u, 0u, 0u);
    if (text != 0u) g_call1(g, kTextSetUp, text);
    return text;
}

// Each frame, after the game placed its own: our texts made or changed, and
// placed.
void room_list_frame(Guest *g, gaddr parts, gaddr texts) {
    Shown &s = shown();
    bool same = s.texts == texts;
    for (std::uint32_t id = 0; same && id < kOurTexts; ++id)
        same = s.made[id] == g_ld32(g, texts + 4u * id);
    if (!same) s = Shown{texts, {}, {}};  // another room list: nothing of ours on it

    std::array<std::string, kOurTexts> says;
    const std::string server = portablekit::adhoc_server_address();
    net::relay::Where where;
    // Nobody online: the server has not counted this player yet.
    if (!server.empty() && online::where(server, where) && where.online != 0u) says = wanted(where);

    bool changed = false;
    for (std::uint32_t id = 0; id < kOurTexts; ++id) {
        if (says[id] == s.says[id]) continue;
        changed = true;
        if (s.made[id] != 0u) {
            release(g, parts, part_of(id), s.made[id]);
            g_st32(g, texts + 4u * id, 0u);
            s.made[id] = 0u;
        }
        s.says[id].clear();
        if (says[id].empty() || g_ld32(g, parts + 4u * part_of(id)) == 0u) continue;
        const gaddr text = text_from(g, says[id]);
        if (text == 0u) continue;
        g_st32(g, texts + 4u * id, text);
        g_call8(g, kTextAttach, parts, texts, id, kRightMiddle, part_of(id), 0u, 0u, 0u);
        s.made[id] = text;
        s.says[id] = says[id];
    }
    if (changed) {
        std::string rooms;
        for (std::uint32_t id = 0; id < kRooms; ++id)
            if (!s.says[id].empty()) rooms += (rooms.empty() ? "" : ", ") + std::to_string(id + 1u) + ": " + s.says[id];
        std::cout << "[lobby-status] " << (s.says[kSummaryText].empty() ? std::string("nothing known") : s.says[kSummaryText])
                  << (rooms.empty() ? "" : "; rooms " + rooms) << std::endl;
    }

    for (std::uint32_t id = 0; id < kOurTexts; ++id) {
        const gaddr text = s.made[id];
        if (text == 0u || g_ld32(g, parts + 4u * part_of(id)) == 0u) continue;
        // The hand moved: the bars' lists were made again without it.
        if (!listed(g, parts, part_of(id), text))
            g_call8(g, kTextAttach, parts, texts, id, kRightMiddle, part_of(id), 0u, 0u, 0u);
        if (id < kRooms) {
            // As the bar's name: at locator 0, or on the chosen bar (its lit
            // copy on the list) at locator 1, where menu 0x08A0C0EC puts the
            // name in white over the lit copy's dark shadow at locator 0.
            const gaddr lit = g_ld32(g, texts + 4u * (kFirstLitCopy + id));
            const std::uint32_t locator = lit != 0u && listed(g, parts, part_of(id), lit) ? 1u : 0u;
            g_call4(g, kPlaceText, parts, part_of(id), locator, text);
            g_st16(g, text + 0x14u, static_cast<std::uint16_t>(g_ld16s(g, text + 0x14u) + kCountRight));
        } else {
            g_call4(g, kPlaceText, parts, part_of(id), 0u, text);
            g_st16(g, text + 0x14u, static_cast<std::uint16_t>(kSummaryX));
            g_st16(g, text + 0x16u, static_cast<std::uint16_t>(kSummaryY));
            for (std::uint32_t i = 0; i < 3u; ++i) g_st8(g, text + 0x0Cu + i, kHintInk[i]);
        }
    }
}

// main 0x088D44EC, when local versus does not take it.
void run_place_texts(Guest *g, const decomp::CallState &in, GuestRegs &) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    decomp::call_original(g, kPlaceTexts, regs);
    texts_placed(g, in);
}

struct Install {
    Install() {
        if (off() || versus::enabled()) return;
        static const decomp::Override overrides[] = {
            {kPlaceTexts, "lobby_status_place_texts", decomp::Module::Main, decomp::Returns::Nothing, &run_place_texts},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
    }
} install;

} // namespace

void texts_placed(Guest *g, const decomp::CallState &in) {
    if (off() || in.gpr[31] != kRoomListPlaced || in.gpr[6] != kRoomListTexts) return;
    // The menu overlay's room list, not the battle overlay at the same place.
    if (g_ld32(g, kRoomListPlaced - 8u) != kRoomListCall || g_ld32(g, kRoomListPlaced - 4u) != kRoomListTable) return;
    room_list_frame(g, in.gpr[4], in.gpr[5]);
}

} // namespace d012::lobby_status
