// Local versus: player 2 on a second controller (docs/port/local-versus.md,
// section 1). Everything below acts only while versus is active (chosen from
// the Battle Mode row, or forced with D012_LOCAL_VERSUS=1).
//
// PortableKit opens a second gamepad as player 2's while a port wants one
// (hle_extension::want_second_pad); the game's own pad read (sceCtrl) stays
// player 1's. This file gives player 2's pad to the game in three places:
//
//   - Raw pad 1 (pad manager 0x08C15B90 + 0x60). It reads no hardware; the
//     ad hoc code feeds it by writing its embedded SceCtrlData (+0x40) and
//     setting its mode (+0x50) to 1, and so does this, at every pad read.
//     Logical pad 1, which the battle set-up gives the opponent's fighter
//     and its camera (main 0x0888BA0C), reads it.
//   - The game's own pad while the opponent is picked: every menu reads
//     logical pad 0, so while the character room (menu overlay, object
//     *0x08AA3F44) is in its opponent states 8 to 11, the pad read gets
//     player 2's pad instead of player 1's. The same while player 2 is in
//     Customize, and on the way there and back (d012_versus_rules.hpp).
//   - Player 2's Customize. On their pick, triangle gives the grid's
//     Customize, which the game's opponent pick (menu 0x08A984BC, an
//     override in the menu overlay) has no case for: the room waited for
//     ever. The override starts it as player 1's pick starts its own, for
//     player 2's character, and refuses it where player 1's pick would. On
//     the setup screen, player 2's triangle goes into the game's pad read
//     as a press of the setup's own Customize, which is then given player
//     2's character.
//   - Player 2's pad lost in battle: START on raw pad 1 for one read, so the
//     game's own pause opens as player 2's (the battle checks START on each
//     fighter's pad, battle 0x08B20458, and the pause menu answers the pad
//     that opened it) and waits for player 2's pad to return. Player 1's
//     gamepad lost the same way: START in the game's own pad for one read.
//     A notice over the battle says which controller to plug in again;
//     player 1's goes too once they press anything on the keyboard.
//   - The opponent's AI mode: the battle set-up switches slot 1 to the CPU
//     through battle 0x08B2F2E4(fighter, 0) (calls at main 0x0888BE5C and
//     0x0888BE90). In a Quick Battle (FIELD+0x4C == 4) that call asks for
//     mode 1, the player's, instead. The battle's intro and outro still hand
//     both fighters to the CPU and back, as they do player 1's.
//
// R3 on player 2's pad presses SELECT on raw pad 1, the camera's recentre,
// while player 2's battle camera takes input (as d012_camera.cpp does for
// player 1).
//
// MIT licence; see port/README.md.

#include "d012_versus.hpp"
#include "snapshot.hpp"
#include "d012_decomp_harness.hpp"
#include "d012_input.hpp"
#include "d012_rollback.hpp"
#include "d012_sync.hpp"
#include "d012_versus_rules.hpp"

#include "hle/hle_common.hpp"
#include "install/user_data.hpp"
#include "portablekit/hle_extension.hpp"

#include "psprecomp/runtime.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "imgui.h"
#include "ui/ui.hpp"
#include "ui/widgets.hpp"
#endif

#include <any>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace d012::versus {
namespace {

namespace ext = portablekit::hle_extension;
namespace rules = d012::versus_rules;

constexpr std::uint32_t kSceCtrlReadBufferPositive = 0x1F803938u;
constexpr std::uint32_t kSelect = 0x0001u;
constexpr std::uint32_t kStart = 0x0008u;
// Pad reads within which player 2's camera must have taken input for R3 to
// press SELECT (one read a frame).
constexpr std::uint32_t kLiveReads = 4u;

// main
constexpr std::uint32_t kRawPad1 = 0x08C15BF0u;
constexpr std::uint32_t kRawInjectButtons = 0x44u;
constexpr std::uint32_t kRawInjectStick = 0x48u;  // lx, ly
constexpr std::uint32_t kRawMode = 0x50u;
constexpr std::uint32_t kBattleManager = 0x089D6A10u;  // -> FIELD
constexpr std::uint32_t kFieldMode = 0x4Cu;
constexpr std::uint32_t kFieldOpponent = 0x3Cu;
constexpr std::uint32_t kModeQuickBattle = 4u;
constexpr std::uint32_t kFieldSetup = 0x0888BA0Cu;
constexpr std::uint32_t kFieldSetupSize = 1596u;
// battle
constexpr std::uint32_t kSetAiMode = 0x08B2F2E4u;   // (fighter, mode) -> 0, -1, -2, -3
constexpr std::uint32_t kAiSetMode = 0x08A64078u;   // (ai, mode)
constexpr std::uint32_t kFighterAi = 0x5C4u;
constexpr std::uint32_t kAiHuman = 1u;
// menu: the character room
constexpr std::uint32_t kRoomObject = 0x08AA3F44u;
// Its state table: 8-byte entries, {s16 this-adjustment, s16, function}; the
// first three functions tell the menu overlay from the battle overlay, which
// shares its addresses.
constexpr std::uint32_t kRoomStates = 0x08AA3F48u;
constexpr std::uint32_t kRoomStateSignature[] = {0x08A9DA38u, 0x08A98188u, 0x08A9DD14u};
constexpr std::uint32_t kRoomWork = 0x154u;
constexpr std::uint32_t kRoomState = 0x910u;
// The room's work area: where it goes after Customize, its flags, the pick
// grid, the character Customize opens, the opponent's character, and the
// settings handed to Customize.
constexpr std::uint32_t kRoomBackTo = 0x918u;
constexpr std::uint32_t kRoomFlags = 0x91Cu;
constexpr std::uint32_t kRoomGrid = 0x92Cu;
constexpr std::uint32_t kRoomCustomized = 0x8048u;
constexpr std::uint32_t kRoomOpponent = 0x8050u;
constexpr std::uint32_t kRoomCustomizeSettings = 0x8060u;
// The setup object (+0x978), its state (+0x1EE4) and its result (+0x558).
constexpr std::uint32_t kRoomSetup = 0x978u;
constexpr std::uint32_t kSetupState = 0x1EE4u;
constexpr std::uint32_t kSetupResult = 0x558u;

struct State {
    std::uint32_t reads = 0;
    std::uint32_t camera_live_read = 0xFFFF0000u;
    ext::SecondPad pad;
    bool wanted = false;
    // Told to PortableKit: player 2's gamepad stays player 2's while versus is on.
    bool playing = false;
    bool connected = false;
    bool picking = false;
    bool announced_human = false;
    // Pad reads left with START pressed on raw pad 1: player 2's pad went
    // away in battle, and the game's pause, as player 2's, waits for it.
    std::uint32_t pause_reads = 0;
    // The same for player 1's gamepad, on the game's own pad. A player on the
    // keyboard (player 1's, or player 2's by Controls > Player 2) has no
    // gamepad to lose.
    bool first_connected = false;
    std::uint32_t pause1_reads = 0;
    // Whose controller went away in this battle (kLostPlayer1, kLostPlayer2),
    // for the notice; cleared as each comes back and when the battle ends.
    std::uint32_t lost = 0;
    // In a versus battle, player 1's fighter played (not the CPU's, as in the
    // intro), at the last pad read: the 1P / 2P labels show.
    bool fighting = false;
    // When the last battle frame was drawn split (note_split_frame), and
    // whether it was. A time, not a pad read: the loading after a battle
    // reads no pad.
    std::chrono::steady_clock::time_point split_time;
    bool split = false;
    // When player 2's pick began, for the header's fade.
    std::chrono::steady_clock::time_point picking_time;
    std::uint32_t fallen = 0;  // bit 0 player 1's fighter, bit 1 player 2's
    // The battle has reached its finishing blow (phase 1 on): no tags again
    // until the next fight (read_fighting).
    bool ended = false;
    bool chosen = false;      // the Battle Mode row was chosen
    bool start_menu = false;  // the start menu is up (player 2's pad looked for)
    // Pad reads left for the notice "Connect a second controller".
    std::uint32_t ask_reads = 0;
    // Player 2's way to Customize and back, and whether the game's pad had
    // player 2's at the last read.
    rules::Trip trip;
    bool player2_menu = false;
    // Player 2's buttons at the last read, for presses.
    std::uint32_t p2_before = 0;
    // Player 2's triangle went to the setup screen at the last read, with
    // the room's flags and the setup's result then; and reads left in which
    // the room is to take the setup's Customize, which is player 2's.
    bool setup_press = false;
    std::uint32_t setup_flags = 0;
    std::uint32_t setup_result = 0;
    std::uint32_t setup_wait = 0;
    // Player 2's Customize is on screen (the room in state 52 on player 2's
    // trip), and since when, for its "Player 2" plate.
    bool customizing = false;
    std::chrono::steady_clock::time_point customizing_time;
    // Player vs Player (Party): versus is on and the start menu left for
    // Quick Battle (Party) (its destination, main 0x09B6D838, is 3 until the
    // menu leaves for another).
    bool party = false;
    // The party battle: each side's members (the battle request's entries),
    // fights won, whether this fight's winner is counted, how the last fight
    // went on (its phase 14 or 16), the battle overlay at the last read, and
    // once the battle is over (the game's final results) its winner: 1 player
    // 1, 2 player 2, 3 a draw.
    std::uint32_t party_members[2] = {};
    std::uint32_t party_wins[2] = {};
    bool fight_counted = false;
    std::uint32_t fight_went_on = 0;
    bool party_in_battle = false;
    std::uint32_t party_winner = 0;
    // A fight played at the last read, for the pause of a fight that starts
    // with a controller gone (lost between fights), and the reads left until
    // it is paused (the game takes START once the fight is under way).
    bool was_fighting = false;
    std::uint32_t pause_in = 0;
    // The room is on the party's battle setup (states 28 and 29).
    bool party_setup = false;
};

constexpr std::uint32_t kDestination = 0x09B6D838u;
constexpr std::uint32_t kDestinationParty = 3u;

constexpr std::uint32_t kLostPlayer1 = 1u;
constexpr std::uint32_t kLostPlayer2 = 2u;

State &state() {
    static State s;
    return s;
}

void add_state_to_snapshots() {
    portablekit::snapshot::add(
        "versus", [] { return std::any(state()); },
        [](const std::any &any) { state() = std::any_cast<const State &>(any); });
}
const bool added_to_snapshots = (add_state_to_snapshots(), true);

// The split task runs every battle frame; a split frame drawn within this
// long is a split battle on screen.
bool split_now(const State &s) {
    return s.split && std::chrono::steady_clock::now() - s.split_time < std::chrono::milliseconds(150);
}

bool user_ram(std::uint32_t address) {
    const std::uint32_t a = address & 0x1FFFFFFFu;
    return a >= 0x08800000u && a < 0x0A000000u - 0x1000u;
}

// The battle overlay is in the overlay window (battle 0x08B2F2E4's first two
// instructions), so a battle is on.
bool battle_resident(psprecomp::GuestMemory &memory) {
    return memory.load32(kSetAiMode) == 0x27BDFFF0u && memory.load32(kSetAiMode + 4u) == 0x2CA60006u;
}

// The character room, when it is up: its work area, state and flags.
struct Room {
    bool present = false;
    std::uint32_t work = 0, state = 0, flags = 0;
    std::uint32_t next = 0;  // +0x914, where the waiting state 19 goes
};

Room read_room(psprecomp::GuestMemory &memory) {
    for (std::uint32_t i = 0; i < 3u; ++i)
        if (memory.load32(kRoomStates + 8u * i + 4u) != kRoomStateSignature[i]) return {};
    const std::uint32_t room = memory.load32(kRoomObject);
    if (!user_ram(room)) return {};
    const std::uint32_t work = memory.load32(room + kRoomWork);
    if (!user_ram(work)) return {};
    return {true, work, memory.load32(work + kRoomState), memory.load32(work + kRoomFlags), memory.load32(work + 0x914u)};
}

// The setup screen's result (rules::kSetupCustomize), or 0 with no setup.
std::uint32_t setup_result(psprecomp::GuestMemory &memory, const Room &room) {
    if (!room.present) return 0u;
    const std::uint32_t setup = memory.load32(room.work + kRoomSetup);
    const std::uint32_t state = user_ram(setup) ? memory.load32(setup + kSetupState) : 0u;
    return user_ram(state) ? memory.load32(state + kSetupResult) : 0u;
}

// The character room is on screen and picking the opponent: the 1-on-1's, or
// the party's (Quick Battle (Party), the CPU's party).
bool picking_opponent(const Room &room) { return room.present && rules::player2_pick(room.state, room.next); }

// The game's character ids that Customize takes (d012_versus_rules.hpp).
bool customizable_character(std::uint32_t character) {
    const auto id = static_cast<std::int32_t>(character);
    return rules::customizable(id, id);
}

// The header over the opponent's grid. When the room starts the opponent's
// pick (state 8, menu 0x08A98458), menu 0x08AA0D48 builds the grid with a
// header animation from a table by mode: 2 ("Player Select") at
// 0x08AA3C84 for the player, 6 ("CPU Select") at 0x08AA3C98 for the CPU in
// modes 0 to 2, 4 in modes 3 and 4. In versus, the CPU's entries take
// player 2's header; otherwise they are put back. Written only while the
// menu overlay is the one loaded (the room's state table is there).
constexpr std::uint32_t kOpponentHeaders = 0x08AA3C98u;
constexpr std::uint32_t kStockOpponentHeader = 6u;

void set_opponent_header(psprecomp::GuestMemory &memory) {
    for (std::uint32_t i = 0; i < 3u; ++i)
        if (memory.load32(kRoomStates + 8u * i + 4u) != kRoomStateSignature[i]) return;
    // 4, as modes 3 and 4 have it for the opponent too: "Player Select",
    // with the look and the set, as the player's grid has them (2 looks the
    // same). No header says player 2's: pick_header_draw() draws one over it.
    const std::uint32_t header = active() ? 4u : kStockOpponentHeader;
    for (std::uint32_t mode = 0; mode < 3u; ++mode)
        if (memory.load32(kOpponentHeaders + 4u * mode) != header) memory.store32(kOpponentHeaders + 4u * mode, header);
}

// Player 1's character records (save root *0x089D712C + 0xF58, 31 of
// 0x778 bytes; docs/research/roster-and-save.md 1.6) with a look of -1 at
// +0x774. A record's look is 1 ("Normal") or more; with -1 the character's
// panel on a pick (the player's, or player 2's, which reads player 1's
// records) keeps the last character's name and draws no portrait. Saves from
// earlier versus builds have one (the owner's Cecil, a test copy's
// Lightning); it is set back to 1 while the character room is up, and the
// game saves it with its next autosave. D012_TRACE_RECORDS=1 also logs every
// record's look, set and byte +2 as they change.
void repair_looks(psprecomp::GuestMemory &memory, const State &s, bool battle) {
    const std::uint32_t root = memory.load32(0x089D712Cu);
    if (!user_ram(root)) return;
    static const bool trace = std::getenv("D012_TRACE_RECORDS") != nullptr;
    static std::uint32_t last[31] = {};
    for (std::uint32_t i = 0; i < 31u; ++i) {
        const std::uint32_t record = root + 0xF58u + i * 0x778u;
        const std::int8_t id = static_cast<std::int8_t>(memory.load8(record));
        if (!battle && id > 0 && static_cast<std::int8_t>(memory.load8(record + 0x774u)) < 0) {
            memory.store8(record + 0x774u, 1u);
            std::cout << "[versus] character " << int(id) << "'s record had no look: set back to Normal" << std::endl;
        }
        if (!trace) continue;
        const std::uint32_t now = memory.load8(record + 0x774u) | memory.load8(record + 0x773u) << 8 |
                                  memory.load8(record + 2u) << 16 | static_cast<std::uint32_t>(memory.load8(record)) << 24;
        if (last[i] != 0u && now != last[i])
            std::printf("[versus] read %u record %u (id %d): look %d set %d byte2 %u (was %08X)%s\n", s.reads, i,
                        static_cast<std::int8_t>(now >> 24), static_cast<std::int8_t>(now),
                        static_cast<std::int8_t>(now >> 8), (now >> 16) & 0xFFu, last[i], battle ? " in battle" : "");
        last[i] = now;
    }
    if (trace) std::fflush(stdout);
}

// No records from a versus battle (0.1): rewards are left out (below), but
// the ending still writes other things to player 1's save image (save root
// *0x089D712C + 0xF08, 0x3ECC8 bytes; docs/research/roster-and-save.md 1.6):
// a character record's byte +2, the play-style AI's learning level (0 to 150
// in an 11-step table at 0x089CBCC8, read by main 0x088CBB60), counts up, and
// the per-character and total battle statistics change. Both are in the
// image's section 1 (characters, inventory, statistics, play styles: image
// +0x50, 0x1CEAC bytes), which is kept as the battle starts (phase 0, once
// a battle; a rematch keeps the first) and put back in the buffer the
// savedata utility is handed while the battle is on
// (sceUtilitySavedataInitStart, a save mode), and in the image itself once
// the battle overlay has gone. The rest of the image (a running counter at
// +0x1CFD4, the play time or the like) is left as the game has it.
constexpr std::uint32_t kSaveRoot = 0x089D712Cu;
constexpr std::uint32_t kSaveImage = 0xF08u;
constexpr std::uint32_t kSaveImageSize = 0x3ECC8u;
constexpr std::uint32_t kSaveSection1 = 0x50u;
constexpr std::uint32_t kSaveSection1Size = 0x1CEACu;

std::vector<std::uint8_t> &save_before() {
    static std::vector<std::uint8_t> image;
    return image;
}

std::uint32_t save_image(psprecomp::GuestMemory &memory) {
    const std::uint32_t root = memory.load32(kSaveRoot);
    return user_ram(root) ? root + kSaveImage : 0u;
}

// Section 1 put back in the image at `at`; the bytes that differed, logged
// with the first few ranges (image offsets).
void put_save_back(psprecomp::GuestMemory &memory, std::uint32_t at, const char *where) {
    std::vector<std::uint8_t> &before = save_before();
    std::vector<std::uint8_t> now(before.size());
    at += kSaveSection1;
    memory.copy_out(at, now);
    std::uint32_t changed = 0;
    std::string ranges;
    for (std::uint32_t i = 0; i < now.size(); ++i) {
        if (now[i] == before[i]) continue;
        if (changed++ < 4096u && ranges.size() < 160u && (i == 0u || now[i - 1u] == before[i - 1u])) {
            char text[16];
            std::snprintf(text, sizeof text, " +%05X", kSaveSection1 + i);
            ranges += text;
        }
    }
    if (changed == 0u) return;
    memory.copy_in(at, before);
    std::cout << "[versus] " << changed << " bytes of the save put back as they were before the battle (" << where
              << "):" << ranges << std::endl;
}

void guard_save(psprecomp::GuestMemory &memory, bool battle) {
    std::vector<std::uint8_t> &before = save_before();
    const std::uint32_t image = save_image(memory);
    if (!battle) {
        if (!before.empty() && image != 0u) put_save_back(memory, image, "after the battle");
        before.clear();
        return;
    }
    if (!before.empty() || image == 0u) return;
    const std::uint32_t flow = memory.load32(0x08B0CF20u);
    const std::uint32_t phase = user_ram(flow) ? memory.load32(flow + 8u) : 0xFFFFFFFFu;
    if (phase != 0u) return;
    before.resize(kSaveSection1Size);
    memory.copy_out(image + kSaveSection1, before);
}

constexpr std::uint32_t kSavedataInitStart = 0x50C4CD57u;
constexpr std::uint32_t kSavedataMode = 0x30u;
constexpr std::uint32_t kSavedataBuffer = 0x74u;
constexpr std::uint32_t kSavedataBufferSize = 0x78u;

void feed_raw_pad1(psprecomp::GuestMemory &memory, State &s) {
    std::uint32_t buttons = s.pad.connected ? s.pad.buttons : 0u;
    if (s.pad.connected && (s.pad.extra & ext::kExtraR3) != 0u && s.reads - s.camera_live_read <= kLiveReads)
        buttons |= kSelect;
    if (s.pause_reads != 0u) {
        --s.pause_reads;
        buttons |= kStart;
    }
    memory.store32(kRawPad1 + kRawInjectButtons, buttons);
    memory.store8(kRawPad1 + kRawInjectStick, s.pad.connected ? s.pad.analog_x : 0x80u);
    memory.store8(kRawPad1 + kRawInjectStick + 1u, s.pad.connected ? s.pad.analog_y : 0x80u);
    memory.store32(kRawPad1 + kRawMode, 1u);
}

// D012_TRACE_VERSUS=1: every 15 pad reads in battle, both fighters'
// positions (fighter +0x30, +0x34, +0x38), the opponent's AI mode (ai
// +0x54: 1 the player's, 0 the CPU's) and what raw pad 1 was given.
void trace_fighters(psprecomp::GuestMemory &memory, const State &s) {
    static const bool on = [] {
        const char *text = std::getenv("D012_TRACE_VERSUS");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    if (!on || s.reads % 15u != 0u) return;
    const std::uint32_t field = memory.load32(kBattleManager);
    if (!user_ram(field)) return;
    const std::uint32_t player = memory.load32(field + 0x38u);
    const std::uint32_t opponent = memory.load32(field + kFieldOpponent);
    if (!user_ram(player) || !user_ram(opponent)) return;
    const auto f = [&](std::uint32_t address) {
        const std::uint32_t word = memory.load32(address);
        float value;
        std::memcpy(&value, &word, 4);
        return value;
    };
    const std::uint32_t ai = memory.load32(opponent + kFighterAi);
    // Each fighter's camera eye (camera +0x00) and its distance to the fighter.
    const auto eye = [&](std::uint32_t fighter, float out[4]) {
        const std::uint32_t camera = memory.load32(fighter + 0x5ACu);
        if (!user_ram(camera)) {
            out[0] = out[1] = out[2] = out[3] = 0.0f;
            return;
        }
        for (int k = 0; k < 3; ++k) out[k] = f(camera + 4u * static_cast<std::uint32_t>(k));
        const float dx = out[0] - f(fighter + 0x30u), dy = out[1] - f(fighter + 0x34u), dz = out[2] - f(fighter + 0x38u);
        out[3] = std::sqrt(dx * dx + dy * dy + dz * dz);
    };
    float e1[4], e2[4];
    eye(player, e1);
    eye(opponent, e2);
    // Each fighter's map action (+0x6E0: -1 none, 0 a wall, 1 and 2 other
    // freerun places, 3 a held one) and its state (*(+0x3A4)).
    const auto action = [&](std::uint32_t fighter) { return static_cast<int>(memory.load32(fighter + 0x6E0u)); };
    const auto state = [&](std::uint32_t fighter) {
        const std::uint32_t at = memory.load32(fighter + 0x3A4u);
        return user_ram(at) ? static_cast<int>(memory.load32(at)) : -1;
    };
    std::printf("[versus] read %u p1 (%.2f, %.2f, %.2f) p2 (%.2f, %.2f, %.2f) p2 ai %d pad1 0x%04X %u,%u "
                "eye1 (%.2f, %.2f, %.2f) d %.2f eye2 (%.2f, %.2f, %.2f) d %.2f action %d/%d state 0x%X/0x%X "
                "local 0x%02X/0x%02X\n",
                s.reads, f(player + 0x30u), f(player + 0x34u), f(player + 0x38u), f(opponent + 0x30u),
                f(opponent + 0x34u), f(opponent + 0x38u),
                user_ram(ai) ? static_cast<int>(memory.load32(ai + 0x54u)) : -1,
                memory.load32(kRawPad1 + kRawInjectButtons), memory.load8(kRawPad1 + kRawInjectStick),
                memory.load8(kRawPad1 + kRawInjectStick + 1u), e1[0], e1[1], e1[2], e1[3], e2[0], e2[1], e2[2], e2[3],
                action(player), action(opponent), state(player), state(opponent), memory.load8(player + 0x5B0u),
                memory.load8(opponent + 0x5B0u));
    std::fflush(stdout);
}

// D012_TRACE_PARTY=1, versus chosen or not: the character room's state and a
// few of its fields, the start menu's destination, the battle's FIELD mode
// and phase, and the side set-ups' characters and controller types, each
// logged as it changes (for Player vs Player (Party), local-versus.md 0.2).
bool trace_party_on() {
    static const bool on = [] {
        const char *text = std::getenv("D012_TRACE_PARTY");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    return on;
}

void trace_party(psprecomp::GuestMemory &memory, const State &s, bool battle) {
    if (!trace_party_on()) return;
    constexpr std::uint32_t kSides = 0x09AFB8A0u, kSideSize = 0x1060u, kSideControl = 0xF34u;
    static std::uint32_t last_room = 0xFFFFFFFFu, last_destination = 0xFFFFFFFFu, last_mode = 0xFFFFFFFFu;
    static std::uint32_t last_phase = 0xFFFFFFFFu, last_sides = 0xFFFFFFFFu;
    static bool last_battle = false;
    bool menu = true;
    for (std::uint32_t i = 0; i < 3u; ++i)
        if (memory.load32(kRoomStates + 8u * i + 4u) != kRoomStateSignature[i]) menu = false;
    std::uint32_t room = 0xFFFFFFFFu, work = 0u;
    if (menu) {
        const std::uint32_t object = memory.load32(kRoomObject);
        work = user_ram(object) ? memory.load32(object + kRoomWork) : 0u;
        if (user_ram(work)) room = memory.load32(work + kRoomState);
    }
    if (room != last_room) {
        last_room = room;
        if (room != 0xFFFFFFFFu)
            std::printf("[party] read %u room state %u (+0x914 %u, +0x918 %u, +0x91C 0x%X, +0x970 %u, +0x8048 %d, "
                        "+0x804C %d, +0x8050 %d, +0x8058 %u, +0x81B4 0x%X)\n",
                        s.reads, room, memory.load32(work + 0x914u), memory.load32(work + 0x918u),
                        memory.load32(work + 0x91Cu), memory.load8(work + 0x970u),
                        static_cast<std::int32_t>(memory.load32(work + 0x8048u)),
                        static_cast<std::int32_t>(memory.load32(work + 0x804Cu)),
                        static_cast<std::int32_t>(memory.load32(work + 0x8050u)), memory.load8(work + 0x8058u),
                        memory.load32(work + 0x81B4u));
        else
            std::printf("[party] read %u room gone\n", s.reads);
    }
    const std::uint32_t destination = memory.load32(0x09B6D838u);
    if (destination != last_destination) {
        last_destination = destination;
        std::printf("[party] read %u destination %u\n", s.reads, destination);
    }
    if (battle != last_battle) {
        last_battle = battle;
        std::printf("[party] read %u battle overlay %s\n", s.reads, battle ? "in" : "out");
    }
    const std::uint32_t field = battle ? memory.load32(kBattleManager) : 0u;
    const std::uint32_t mode = user_ram(field) ? memory.load32(field + kFieldMode) : 0xFFFFFFFFu;
    if (mode != last_mode) {
        last_mode = mode;
        if (mode != 0xFFFFFFFFu)
            std::printf("[party] read %u FIELD mode %u (+0x50 %u, +0x54 %u, fighters 0x%08X 0x%08X)\n", s.reads, mode,
                        memory.load32(field + 0x50u), memory.load32(field + 0x54u), memory.load32(field + 0x38u),
                        memory.load32(field + 0x3Cu));
    }
    const std::uint32_t flow = battle ? memory.load32(0x08B0CF20u) : 0u;
    const std::uint32_t phase = user_ram(flow) ? memory.load32(flow + 8u) : 0xFFFFFFFFu;
    if (phase != last_phase) {
        last_phase = phase;
        if (phase < 64u) std::printf("[party] read %u battle phase %u\n", s.reads, phase);
    }
    // The side set-ups: character id (+0), level (+1), controller type (+0xF34).
    std::uint32_t sides = 0u;
    for (std::uint32_t i = 0; i < 2u; ++i)
        sides = sides * 31u + memory.load8(kSides + i * kSideSize) + 7u * memory.load16(kSides + i * kSideSize + kSideControl);
    if (sides != last_sides) {
        last_sides = sides;
        std::printf("[party] read %u sides:", s.reads);
        for (std::uint32_t i = 0; i < 2u; ++i) {
            const std::uint32_t setup = kSides + i * kSideSize;
            std::printf(" %u:%d/L%u/c%d", i, static_cast<std::int8_t>(memory.load8(setup)), memory.load8(setup + 1u),
                        static_cast<std::int16_t>(memory.load16(setup + kSideControl)));
        }
        std::printf("\n");
    }
    // The battle request (*0x089D8508): per side 0xEA8 bytes, five entries
    // of 0x2E8, each with its source type at +0x68 and arguments from +0x78
    // (roster-and-save.md 1.7).
    static std::uint32_t last_request = 0xFFFFFFFFu;
    const std::uint32_t request = memory.load32(0x089D8508u);
    if (user_ram(request)) {
        std::uint32_t sum = request;
        for (std::uint32_t side = 0; side < 2u; ++side)
            for (std::uint32_t n = 0; n < 5u; ++n) {
                const std::uint32_t entry = request + side * 0xEA8u + n * 0x2E8u;
                sum = sum * 31u + memory.load32(entry + 0x68u) * 7u + memory.load32(entry + 0x78u);
            }
        if (sum != last_request) {
            last_request = sum;
            std::printf("[party] read %u request 0x%08X:", s.reads, request);
            for (std::uint32_t side = 0; side < 2u; ++side) {
                std::printf(" side %u", side);
                for (std::uint32_t n = 0; n < 5u; ++n) {
                    const std::uint32_t entry = request + side * 0xEA8u + n * 0x2E8u;
                    std::printf(" [%u: %08X %08X %08X]", n, memory.load32(entry + 0x68u), memory.load32(entry + 0x78u),
                                memory.load32(entry + 0x7Cu));
                }
            }
            std::printf("\n");
        }
    }
    std::fflush(stdout);
}

// ---- Player vs Player (Party): the game's words for the parties ----------
//
// The party screens are worded for player 1 against the CPU. Their words are
// the game's messages, decoded in RAM once their table is loaded (a table:
// u16 count, u16 offsets, then each message: a u16 byte count with the
// terminator, maybe a few control units, UTF-16 text, 0). In Player vs Player
// (Party) these are put in the messages' own places, the bytes kept and put
// back afterwards (only where they still read as written):
//
//   - "Enemy Party" (a table loaded for good, at 0x098F9B70 on the US disc),
//     the right side's name on each fight's VS screen and the final results,
//     as "Player 2";
//   - "Create a party to fight. *Set CPU strength and levels on the next
//     screen." (the menu overlay's), on player 2's pick, as the player's own
//     "Please choose a member. A party can consist of one or more members.".
//
// They are found by their English words, so with the game in another
// language they stay as they are (a few searches, then no more). The red
// "CPU" on the party setup's right plate is no message (every "CPU" among
// them is part of a longer one) and stays.
//
// The left side's name on those screens is player 1's party's ("Party A"),
// from the battle request (*0x089D8508, each side 0xEA8 bytes: the name at
// +0x4C, 14 UTF-16 units; empty on the CPU's side, for which the screens use
// "Enemy Party"); in versus it is written as "Player 1", and the right side's
// as "Player 2", once the room has made the request. The first fight's VS
// screen reads a copy the room has made by then, in the block after the two
// side set-ups (main 0x09AFD960; the name at +0x654 on the US disc): there the
// party's name is written as "Player 1" too, wherever it is in that block.
constexpr std::uint32_t kRequest = 0x089D8508u;
constexpr std::uint32_t kRequestSide = 0xEA8u;
constexpr std::uint32_t kRequestName = 0x4Cu;
constexpr std::uint32_t kRequestNameUnits = 14u;
constexpr std::uint32_t kRequestEntry = 0x2E8u;
constexpr std::uint32_t kRequestEntryType = 0x68u;
constexpr std::uint32_t kUserRam = 0x08800000u, kUserRamEnd = 0x0A000000u;
constexpr std::uint32_t kBattleSetupNames = 0x09AFD960u, kBattleSetupNamesSize = 0x1000u;

struct MessagePatch {
    const char *name = "";
    std::uint32_t at = 0;  // the message's byte count
    std::vector<std::uint8_t> original, patched;
};

std::vector<std::uint16_t> units(const char *text) {
    std::vector<std::uint16_t> out;
    for (const char *c = text; *c != '\0'; ++c) out.push_back(static_cast<std::uint8_t>(*c));
    return out;
}

// The byte count of the message holding `text` at `hit` in `ram` (a copy of
// guest memory from `base`): within three units before the text, with only
// control units (0x8000 and up) between it and the text, its last unit the
// terminator; with `whole`, the text ending the message's words (a
// terminator or a control unit after it). 0 if `hit` is no message's.
std::uint32_t message_at(const std::vector<std::uint8_t> &ram, std::uint32_t base, std::size_t hit, std::size_t length,
                         bool whole) {
    const auto unit = [&](std::size_t i) -> std::uint32_t { return i + 1u < ram.size() ? ram[i] | ram[i + 1u] << 8 : 0xFFFFFFFFu; };
    if (whole) {
        const std::uint32_t after = unit(hit + 2u * length);
        if (after != 0u && after < 0x8000u) return 0u;
    }
    for (std::size_t back = 1; back <= 3u && hit >= 2u * back; ++back) {
        const std::size_t count_at = hit - 2u * back;
        bool controls = true;
        for (std::size_t k = count_at + 2u; k < hit && controls; k += 2u) controls = unit(k) >= 0x8000u;
        if (!controls) break;
        const std::uint32_t bytes = unit(count_at);
        if (bytes % 2u != 0u || bytes < 2u * length + 2u || bytes > 600u) continue;
        const std::size_t end = count_at + 2u + bytes;
        if (end < hit + 2u * length + 2u || unit(end - 2u) != 0u) continue;
        return base + static_cast<std::uint32_t>(count_at);
    }
    return 0u;
}

// Where message `text` is in user RAM (its byte count), or 0.
std::uint32_t find_message(psprecomp::GuestMemory &memory, const char *text, bool whole) {
    static std::vector<std::uint8_t> ram;
    ram.resize(kUserRamEnd - kUserRam - 0x1000u);
    memory.copy_out(kUserRam, ram);
    const std::vector<std::uint16_t> want = units(text);
    for (std::size_t i = 2; i + 2u * want.size() < ram.size(); i += 2u) {
        if (ram[i] != (want[0] & 0xFFu) || ram[i + 1u] != 0u) continue;
        bool match = true;
        for (std::size_t k = 1; k < want.size() && match; ++k)
            match = ram[i + 2u * k] == want[k] && ram[i + 2u * k + 1u] == 0u;
        if (!match) continue;
        if (const std::uint32_t at = message_at(ram, kUserRam, i, want.size(), whole); at != 0u) return at;
    }
    return 0u;
}

// The message at `at` (its byte count) given `words` in place of its text,
// its control units kept, the rest of it zeros. False if they do not fit.
bool patch_message(psprecomp::GuestMemory &memory, MessagePatch &patch, std::uint32_t at, const std::vector<std::uint16_t> &words) {
    const std::uint32_t bytes = memory.load16(at);
    std::vector<std::uint8_t> original(2u + bytes);
    memory.copy_out(at, original);
    std::uint32_t text = at + 2u;
    while (text < at + 2u + bytes && memory.load16(text) >= 0x8000u) text += 2u;
    if (text + 2u * words.size() + 2u > at + 2u + bytes) return false;
    for (std::uint32_t a = text; a < at + 2u + bytes; a += 2u) memory.store16(a, 0u);
    for (std::size_t i = 0; i < words.size(); ++i) memory.store16(text + 2u * static_cast<std::uint32_t>(i), words[i]);
    patch.at = at;
    patch.original = std::move(original);
    patch.patched.resize(patch.original.size());
    memory.copy_out(at, patch.patched);
    std::cout << "[versus] the message \"" << patch.name << "\" at 0x" << std::hex << at << std::dec << " reworded" << std::endl;
    return true;
}

// The message put back, if it still reads as the port wrote it (its table may
// be gone, with something else in its place).
void unpatch_message(psprecomp::GuestMemory &memory, MessagePatch &patch) {
    if (patch.at == 0u) return;
    std::vector<std::uint8_t> now(patch.patched.size());
    memory.copy_out(patch.at, now);
    if (now == patch.patched) memory.copy_in(patch.at, patch.original);
    patch.at = 0u;
}

// Whether a patch is in place and still reads as written.
bool patched(psprecomp::GuestMemory &memory, const MessagePatch &patch) {
    if (patch.at == 0u) return false;
    std::vector<std::uint8_t> now(patch.patched.size());
    memory.copy_out(patch.at, now);
    return now == patch.patched;
}

struct PartyWords {
    MessagePatch enemy{"Enemy Party"};
    MessagePatch create{"Create a party to fight"};
    // Searches made for each in this party battle (or this room visit), at
    // most three, and the pad reads to the next search for "Enemy Party".
    std::uint32_t enemy_searches = 0, enemy_wait = 0;
    std::uint32_t room_searches = 0;
    std::uint32_t named_request = 0;  // the request whose names were written
    // The copy of player 1's party's name written as "Player 1", and what it
    // read before, put back when versus is over.
    std::uint32_t name_copy = 0;
    std::vector<std::uint16_t> name_copy_before;
};

constexpr std::uint32_t kWordSearches = 3u;
constexpr std::uint32_t kWordSearchReads = 300u;

PartyWords &party_words() {
    static PartyWords w;
    return w;
}

// The player's own words for player 2's pick: the player's party pick's
// message, as it is in the same table.
std::vector<std::uint16_t> player_pick_words(psprecomp::GuestMemory &memory) {
    const std::uint32_t at = find_message(memory, "Please choose a member", false);
    if (at == 0u) return {};
    const std::uint32_t bytes = memory.load16(at);
    std::vector<std::uint16_t> words;
    for (std::uint32_t a = at + 2u; a + 2u <= at + 2u + bytes; a += 2u) {
        const std::uint16_t u = memory.load16(a);
        if (u == 0u) break;
        words.push_back(u);
    }
    return words;
}

void write_name(psprecomp::GuestMemory &memory, std::uint32_t at, const char *name) {
    const std::vector<std::uint16_t> words = units(name);
    for (std::uint32_t i = 0; i < kRequestNameUnits; ++i)
        memory.store16(at + 2u * i, i < words.size() ? words[i] : 0u);
}

// Each pad read: the party's words in place while they are wanted.
void party_wording(psprecomp::GuestMemory &memory, State &s, const Room &room, bool battle) {
    PartyWords &w = party_words();
    if (!s.party) {
        unpatch_message(memory, w.enemy);
        unpatch_message(memory, w.create);
        w.named_request = 0u;
        w.enemy_searches = w.enemy_wait = 0u;
        if (w.name_copy != 0u) {
            const std::vector<std::uint16_t> words = units("Player 1");
            bool ours = memory.load16(w.name_copy + 2u * static_cast<std::uint32_t>(words.size())) == 0u;
            for (std::size_t k = 0; k < words.size() && ours; ++k)
                ours = memory.load16(w.name_copy + 2u * static_cast<std::uint32_t>(k)) == words[k];
            if (ours)
                for (std::size_t k = 0; k < w.name_copy_before.size(); ++k)
                    memory.store16(w.name_copy + 2u * static_cast<std::uint32_t>(k), w.name_copy_before[k]);
            w.name_copy = 0u;
        }
        return;
    }
    // "Enemy Party", for as long as the party battle lasts.
    if (!patched(memory, w.enemy)) {
        w.enemy.at = 0u;
        if (w.enemy_wait != 0u) {
            --w.enemy_wait;
        } else if (w.enemy_searches < kWordSearches) {
            ++w.enemy_searches;
            w.enemy_wait = kWordSearchReads;
            if (const std::uint32_t at = find_message(memory, "Enemy Party", true); at != 0u)
                patch_message(memory, w.enemy, at, units("Player 2"));
        }
    }
    // The menu overlay's, while the room is on player 2's pick or after it
    // (the setup, the way to the battle); back on player 1's pick.
    const bool wanted = room.present && room.state >= rules::kPartyPickFirst && room.state < rules::kCustomizeFirst;
    if (!wanted) {
        unpatch_message(memory, w.create);
        w.room_searches = 0u;
    } else if (!patched(memory, w.create) && w.room_searches < kWordSearches) {
        ++w.room_searches;
        w.create.at = 0u;
        const std::vector<std::uint16_t> words = player_pick_words(memory);
        if (const std::uint32_t at = find_message(memory, "Create a party to fight", false); at != 0u && !words.empty())
            patch_message(memory, w.create, at, words);
    }
    // The request's names, once the room has made it as it goes (and again
    // for a new one), before the battle copies them.
    if (room.present) {
        w.named_request = 0u;
        return;
    }
    const std::uint32_t request = memory.load32(kRequest);
    if (!user_ram(request) || request == w.named_request) return;
    // Only a request the room has made (members on both sides).
    std::uint32_t members[2] = {};
    for (std::uint32_t side = 0; side < 2u; ++side)
        for (std::uint32_t n = 0; n < 5u; ++n)
            if (memory.load32(request + side * kRequestSide + n * kRequestEntry + kRequestEntryType) != 0xFFFFFFFFu)
                ++members[side];
    if (members[0] == 0u || members[1] == 0u) return;
    w.named_request = request;
    // Player 1's party's name, as the room wrote it, and its copy.
    std::vector<std::uint16_t> name;
    for (std::uint32_t i = 0; i < kRequestNameUnits; ++i) {
        const std::uint16_t u = memory.load16(request + kRequestName + 2u * i);
        if (u == 0u) break;
        name.push_back(u);
    }
    if (!name.empty() && name != units("Player 1")) {
        for (std::uint32_t at = kBattleSetupNames; at + 2u * kRequestNameUnits < kBattleSetupNames + kBattleSetupNamesSize;
             at += 2u) {
            bool match = memory.load16(at + 2u * static_cast<std::uint32_t>(name.size())) == 0u &&
                         (at == kBattleSetupNames || memory.load16(at - 2u) == 0u);
            for (std::size_t k = 0; k < name.size() && match; ++k)
                match = memory.load16(at + 2u * static_cast<std::uint32_t>(k)) == name[k];
            // "Player 1" and its terminator, where the units after the old
            // name are free (the slot may be no longer than the name needs).
            const std::vector<std::uint16_t> words = units("Player 1");
            for (std::size_t k = name.size() + 1u; k <= words.size() && match; ++k)
                match = memory.load16(at + 2u * static_cast<std::uint32_t>(k)) == 0u;
            if (!match) continue;
            if (w.name_copy == 0u) {
                w.name_copy = at;
                w.name_copy_before.clear();
                for (std::size_t k = 0; k <= words.size(); ++k)
                    w.name_copy_before.push_back(memory.load16(at + 2u * static_cast<std::uint32_t>(k)));
            }
            for (std::size_t k = 0; k <= words.size(); ++k)
                memory.store16(at + 2u * static_cast<std::uint32_t>(k), k < words.size() ? words[k] : 0u);
            std::cout << "[versus] the party's name at 0x" << std::hex << at << std::dec << " written as Player 1" << std::endl;
            at += 2u * kRequestNameUnits;
        }
    }
    for (std::uint32_t side = 0; side < 2u; ++side) {
        s.party_members[side] = members[side];
        write_name(memory, request + side * kRequestSide + kRequestName, side == 0u ? "Player 1" : "Player 2");
    }
    std::cout << "[versus] the party battle: " << s.party_members[0] << " against " << s.party_members[1]
              << ", named Player 1 and Player 2" << std::endl;
}

// The party battle's tally, each read: each fight's winner as it is known
// (read_fighting), and the battle's winner once the game goes to its final
// results. After a fight the battle goes on in one of two ways: phase 14, the
// menu that follows player 1's defeat ("Proceed to next battle" or "Return to
// Battle Setup"), or phase 16, after player 1's win or the last fight, which
// goes on by itself (17, then the next fight's 0). After the last fight the
// battle overlay goes, and the final results are the menu overlay's; after
// "Return to Battle Setup" (a 14) it goes too, with no results. So the battle
// is over when the overlay goes after a 16.
//
// Who won: in a Tournament the side with no members left lost; in a
// Round-Robin the side with more wins won, and equal wins are a draw. So: if
// one side lost every member's fight, the other won; otherwise the wins.
std::uint32_t party_battle_winner(const State &s) {
    const bool out1 = s.party_members[0] != 0u && s.party_wins[1] >= s.party_members[0];
    const bool out2 = s.party_members[1] != 0u && s.party_wins[0] >= s.party_members[1];
    if (out1 != out2) return out1 ? 2u : 1u;
    if (s.party_wins[0] != s.party_wins[1]) return s.party_wins[0] > s.party_wins[1] ? 1u : 2u;
    return 3u;
}

void party_tally(psprecomp::GuestMemory &memory, State &s, const Room &room, bool battle) {
    if (!s.party || room.present) {
        s.party_wins[0] = s.party_wins[1] = 0u;
        s.fight_counted = false;
        s.fight_went_on = 0u;
        s.party_in_battle = false;
        s.party_winner = 0u;
        return;
    }
    const bool left = s.party_in_battle && !battle;
    s.party_in_battle = battle;
    if (left && s.fight_went_on == 16u && s.party_winner == 0u) {
        s.party_winner = party_battle_winner(s);
        std::cout << "[versus] the party battle is over: "
                  << (s.party_winner == 3u ? "a draw" : s.party_winner == 1u ? "player 1 won" : "player 2 won") << std::endl;
    }
    if (!battle) return;
    const std::uint32_t flow = memory.load32(0x08B0CF20u);
    const std::uint32_t phase = user_ram(flow) ? memory.load32(flow + 8u) : 0xFFFFFFFFu;
    if (phase == 0u && s.fallen == 0u) s.fight_counted = false;
    if (phase == 14u || phase == 16u) s.fight_went_on = phase;
    if (s.fallen != 0u && !s.fight_counted) {
        s.fight_counted = true;
        s.fight_went_on = 0u;
        if (s.fallen == 2u) ++s.party_wins[0];
        if (s.fallen == 1u) ++s.party_wins[1];
        std::cout << "[versus] party battle: " << s.party_wins[0] << " - " << s.party_wins[1] << std::endl;
    }
}

// Whether player 1's fighter is played: the intro hands it to the CPU (its
// AI, fighter +0x5C4, in mode 0 at +0x54).
//
// And who has fallen: a fighter's HP is its status block's (fighter +0x52C)
// maximum at +0x0C less the damage taken at +0x08 (as battle 0x089E0110 and
// main 0x08847964 read it), 0 or less once fallen.
void read_fighting(psprecomp::GuestMemory &memory, State &s, bool battle) {
    s.fighting = false;
    s.fallen = 0u;
    if (!battle) s.ended = false;
    const std::uint32_t field = battle ? memory.load32(kBattleManager) : 0u;
    if (!user_ram(field)) return;
    // The battle's phase (*(battle 0x08B0CF20) + 8; 0 the intro and the
    // fight, 1 to 3 the finishing blow, 9 the victory pose, then the
    // results), logged as it changes.
    const std::uint32_t flow = memory.load32(0x08B0CF20u);
    const std::uint32_t phase = user_ram(flow) ? memory.load32(flow + 8u) : 0xFFFFFFFFu;
    {
        static std::uint32_t last = 0xFFFFFFFFu;
        if (phase != last && phase < 32u) {
            last = phase;
            std::cout << "[versus] battle phase " << phase << " at pad read " << s.reads << std::endl;
        }
    }
    const std::uint32_t player = memory.load32(field + 0x38u);
    const std::uint32_t ai = user_ram(player) ? memory.load32(player + kFighterAi) : 0u;
    // Phase 0 is the intro and the fight, 1 on the finishing blow. From the
    // finishing blow on the HUD shows the fighters as its replay has them
    // (the striker's portrait on the left), so the tags go until a new
    // fight's intro hands player 1's fighter to the CPU (phase 0, not
    // played): a rematch does that without leaving the battle overlay, and
    // the fade to the loading screen after a battle does not.
    const bool played = user_ram(ai) && memory.load32(ai + 0x54u) != 0u;
    if (phase >= 1u && phase < 32u) s.ended = true;
    if (phase == 0u && !played) s.ended = false;
    s.fighting = !s.ended && played;
    // Who won, from the victory pose on (phase 9): the flow's winner (+0x20,
    // battle 0x08B09D88). Not before, though a fighter's HP is 0: the fight
    // goes on until the final blow lands (an EX Burst on a fighter at 0 HP).
    if (phase == 0xFFFFFFFFu || phase < 9u || phase >= 32u) return;
    // In a party battle, until the battle goes on (17): the next fight's VS
    // screen is the same battle's.
    if (s.party && phase >= 17u) return;
    const std::uint32_t winner = memory.load32(flow + 0x20u);
    if (winner != 0u && winner == player) {
        s.fallen = 2u;
        return;
    }
    if (winner != 0u && winner == memory.load32(field + 0x3Cu)) {
        s.fallen = 1u;
        return;
    }
    for (std::uint32_t i = 0; i < 2u; ++i) {
        const std::uint32_t fighter = memory.load32(field + 0x38u + 4u * i);
        const std::uint32_t status = user_ram(fighter) ? memory.load32(fighter + 0x52Cu) : 0u;
        if (!user_ram(status)) continue;
        const std::int32_t hp = static_cast<std::int32_t>(memory.load32(status + 0x0Cu)) -
                                static_cast<std::int32_t>(memory.load32(status + 0x08u));
        if (hp <= 0 && memory.load32(status + 0x0Cu) != 0u) s.fallen |= 1u << i;
    }
}

// battle 0x08B2F2E4, with the Quick Battle set-up's CPU call for slot 1
// asking for the player's mode.
void run_set_ai_mode(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    const gaddr fighter = in.gpr[4];
    std::uint32_t mode = in.gpr[5];
    const std::uint32_t caller = in.gpr[31];
    const gaddr field = g_ld32(g, kBattleManager);
    if (trace_party_on())
        std::printf("[party] set AI mode %u on 0x%08X from 0x%08X (FIELD mode %u, fighters 0x%08X 0x%08X)\n", mode, fighter,
                    caller, field != 0u ? g_ld32(g, field + kFieldMode) : 0u, field != 0u ? g_ld32(g, field + 0x38u) : 0u,
                    field != 0u ? g_ld32(g, field + kFieldOpponent) : 0u);
    if (mode == 0u && active() && caller >= kFieldSetup && caller < kFieldSetup + kFieldSetupSize && field != 0u &&
        g_ld32(g, field + kFieldMode) == kModeQuickBattle && g_ld32(g, field + kFieldOpponent) == fighter) {
        mode = kAiHuman;
        State &s = state();
        if (!s.announced_human) {
            s.announced_human = true;
            std::cout << "[versus] the opponent is player 2's" << std::endl;
        }
    }
    if (mode >= 6u) {
        out.v0 = 0xFFFFFFFFu;
        return;
    }
    const gaddr ai = g_ld32(g, fighter + kFighterAi);
    if (ai == 0u) {
        out.v0 = 0xFFFFFFFEu;
        return;
    }
    out.v0 = g_call2(g, kAiSetMode, ai, mode) == 0u ? 0u : 0xFFFFFFFDu;
}

// The same with no versus rule, for the differential test.
void check_set_ai_mode(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    const gaddr fighter = in.gpr[4];
    const std::uint32_t mode = in.gpr[5];
    if (mode >= 6u) {
        out.v0 = 0xFFFFFFFFu;
        return;
    }
    const gaddr ai = g_ld32(g, fighter + kFighterAi);
    if (ai == 0u) {
        out.v0 = 0xFFFFFFFEu;
        return;
    }
    out.v0 = g_call2(g, kAiSetMode, ai, mode) == 0u ? 0u : 0xFFFFFFFDu;
}

// No rewards in a versus battle (docs/port/local-versus.md 0.1). The battle's
// ending (battle 0x08B07324) works out and gives each fighter's side its
// rewards, experience, AP, gil, PP and items (battle 0x08B0AD88, for each
// fighter), then lists them for the local player (battle 0x08B0B71C). In
// versus both are left out; otherwise the game's own run.
constexpr std::uint32_t kGiveRewards = 0x08B0AD88u;
constexpr std::uint32_t kListRewards = 0x08B0B71Cu;

GuestRegs arguments(const decomp::CallState &in) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    return regs;
}

void run_give_rewards(Guest *g, const decomp::CallState &in, GuestRegs &) {
    if (active()) return;
    GuestRegs regs = arguments(in);
    decomp::call_original(g, kGiveRewards, regs);
}

// The list is still started and shown, empty, as the list function starts
// it (battle 0x08B1CF0C and 0x08B1D314 on the list *0x08B1D820) and shows it
// (battle 0x08B1D544): the ending waits for it before the save and the menu.
void run_list_rewards(Guest *g, const decomp::CallState &in, GuestRegs &) {
    if (active()) {
        static bool told = false;
        if (!told) {
            told = true;
            std::cout << "[versus] no rewards for a versus battle" << std::endl;
        }
        const gaddr list = g_ld32(g, 0x08B1D820u);
        const std::uint32_t kind = g_call1(g, 0x088908F0u, in.gpr[4]);
        g_call2(g, 0x08B1CF0Cu, list, kind);
        g_call1(g, 0x08B1D314u, list);
        g_call2(g, 0x08B1D544u, list, 0u);
        return;
    }
    GuestRegs regs = arguments(in);
    decomp::call_original(g, kListRewards, regs);
}

// Player 2's fighter from player 1's save. The battle request's sides are
// built by main 0x0891CFC4 by source type (docs/research/roster-and-save.md
// 1.7): the player's from their save record (type 0, main 0x088F9BB0: the
// character's record with its level, sets, equipment, accessories, costume,
// assist and summons), Quick Battle's opponent by main 0x088FA2AC (type 5,
// a CPU character of the opponent settings' level and equipment). In versus
// the opponent's side is built the CPU's way first, for the controller type
// and the strength fields the field setup and the battle read (setup
// +0xF34..+0xF39), then again from player 1's save record for that
// character with those fields put back: the controller type stays the CPU's,
// so the field setup still asks for the AI mode that becomes player 2's.
constexpr std::uint32_t kBuildCpuSide = 0x088FA2ACu;
constexpr std::uint32_t kBuildSavedSide = 0x088F9BB0u;
constexpr std::uint32_t kSideSetups = 0x09AFB8A0u;
constexpr std::uint32_t kSideSetupSize = 0x1060u;
constexpr std::uint32_t kSideController = 0xF34u;  // s16 type, s16, s16: 6 bytes
constexpr std::uint32_t kRecordCostume = 0x774u;

void run_build_cpu_side(Guest *g, const decomp::CallState &in, GuestRegs &) {
    GuestRegs regs = arguments(in);
    if (trace_party_on())
        std::printf("[party] build CPU side %u character %d from 0x%08X\n", in.gpr[4], static_cast<std::int8_t>(in.gpr[5] & 0xFFu),
                    in.gpr[31]);
    decomp::call_original(g, kBuildCpuSide, regs);
    const std::uint32_t side = in.gpr[4];
    if (!active() || side != 1u) return;
    const gaddr setup = kSideSetups + side * kSideSetupSize;
    std::uint8_t controller[6];
    for (std::uint32_t i = 0; i < 6u; ++i) controller[i] = g_ld8(g, setup + kSideController + i);
    const std::uint32_t character = in.gpr[5] & 0xFFu;
    // (side, character, -, not the save's play-style AI, the set's assist).
    g_call8(g, kBuildSavedSide, side, static_cast<std::uint32_t>(static_cast<std::int8_t>(character)), 0u, 0u,
            0xFFFFFFFFu, 0xFFFFFFFFu, 0u, 0u);
    for (std::uint32_t i = 0; i < 6u; ++i) g_st8(g, setup + kSideController + i, controller[i]);
    // What the caller does with the CPU's entry after this: the costume from
    // the entry (+0x79), and equipment and a set field from the opponent
    // settings when set (+0x88, +0x8A, +0x8C, +0x7F). The entry, the first
    // stack argument less 0x80, takes the record's costume and none of those.
    const gaddr entry = g_ld32(g, in.gpr[29]) - 0x80u;
    g_st8(g, entry + 0x79u, g_ld8(g, setup + kRecordCostume));
    for (const std::uint32_t field : {0x88u, 0x8Au, 0x8Cu}) g_st16(g, entry + field, 0xFFFFu);
    g_st8(g, entry + 0x7Fu, 0xFFu);
    std::cout << "[versus] player 2's " << character << " from player 1's save: level "
              << static_cast<int>(g_ld8(g, setup + 1u)) << ", set " << static_cast<int>(g_ld8s(g, setup + 0x773u))
              << ", costume " << static_cast<int>(g_ld8s(g, setup + kRecordCostume)) << "; controller "
              << g_ld16s(g, setup + kSideController) << " " << g_ld16s(g, setup + kSideController + 2u) << " "
              << g_ld16s(g, setup + kSideController + 4u) << std::endl;
}

// Player 2's Customize from their pick (menu overlay). The room's opponent
// pick update (state 9, menu 0x08A984BC) takes the grid's results 2 (a
// character picked) and 4 (back) only. Player 2's grid is a "Player Select"
// one (set_opponent_header), whose triangle gives result 5, Customize, with
// no check of the cell, and the update then turns the grid's input off and
// waits for ever: the freeze a tester found. The update itself has the way
// out to Customize after a fade (state 10 with state 50 next), but nothing
// starts it. In versus:
//
//   - before the update, triangle on a cell player 1's pick would refuse
//     (Chaos, Feral Chaos, a locked or the random cell; menu 0x08A8255C) is
//     taken off this frame's presses of the game's pad (logical pad 0,
//     pressed at +0x08) with the game's "not allowed" sound;
//   - after it, a result 5 starts what player 1's pick (state 5, menu
//     0x08A9E0E8) does for its own: the character under the cursor is the
//     opponent's (+0x8050) and Customize's (+0x8048), Customize's settings
//     made (main 0x0892A13C), the room flagged for Customize and to come back
//     to player 2's pick (state 8), and the same white fade out, whose end
//     (menu 0x08A9CEC4) lets the update go on to Customize.
//
// Player 2's pad stays the game's through it all (read_pads, rules::room_pad).
//
// Player 2's pick shows no button prompts (the update hides the room's prompt
// bar every frame, menu 0x08A9C5A0); player 1's shows Set, Look and
// Customization while the cursor is on a character (set 0, menu 0x08A9C55C).
// Player 2's shows the setup screen's set 4, "Customization" alone, while the
// cursor is on a character Customize takes: the room's record of the set
// shown (+0x81C4) is -1 to the game's update, so that it leaves the bar
// alone, and 4 otherwise.
//
// D012_NO_PLAYER2_CUSTOMIZE=1 leaves all of it out: the game's own pick and
// setup screen, and the freeze.
bool player2_customize() {
    static const bool on = [] {
        const char *text = std::getenv("D012_NO_PLAYER2_CUSTOMIZE");
        return text == nullptr || *text == '\0' || std::strcmp(text, "0") == 0;
    }();
    return on;
}

constexpr std::uint32_t kPlayer2PickUpdate = 0x08A984BCu;
constexpr std::uint32_t kLogicalPad0Pressed = 0x08C15C50u + 0x08u;
constexpr std::uint32_t kGridWork = 0x28u;
constexpr std::uint32_t kGridState = 0x5E4u;
constexpr std::uint32_t kGridFlags = 0x5E8u;
constexpr std::uint32_t kGridResult = 0x5F8u;
constexpr std::uint32_t kGridBusy = 0x620u;
constexpr std::uint32_t kGridCell = 0x08A7F104u;       // (grid, 2, -1): the cell under the cursor
constexpr std::uint32_t kGridCharacter = 0x08A81188u;  // (grid): its character
constexpr std::uint32_t kMenuSound = 0x088D4BF4u;      // (sound)
constexpr std::uint32_t kSoundRefused = 4u;
constexpr std::uint32_t kCustomizeSettingsInit = 0x0892A13Cu;
constexpr std::uint32_t kFadeFrames = 0x088D4C5Cu;
constexpr std::uint32_t kFadeStart = 0x08873D34u;  // (f12 frames, 3, done, 0, 1, 2)
constexpr std::uint32_t kFadeColour = 0x08873DB0u;  // (rgba floats, 2)
constexpr std::uint32_t kFadeDone = 0x08A9CEC4u;
constexpr std::uint32_t kRoomPromptsShown = 0x81C4u;
constexpr std::uint32_t kShowPrompts = 0x08A9C55Cu;  // (room, set)
constexpr std::uint32_t kHidePrompts = 0x08A9C5A0u;  // (room, 0)
constexpr std::uint32_t kPromptsCustomize = 4u;
constexpr std::uint32_t kPromptsNone = 0xFFFFFFFFu;

// Whether the character under player 2's cursor can be customised.
bool cursor_customizable(Guest *g, gaddr grid) {
    const auto cell = static_cast<std::int32_t>(g_call3(g, kGridCell, grid, 2u, 0xFFFFFFFFu));
    const auto character = static_cast<std::int32_t>(g_call1(g, kGridCharacter, grid));
    return rules::customizable(cell, character);
}

// After the update, on player 2's pick: "Customization" shown while the
// cursor is on a character it takes.
void show_player2_prompts(Guest *g, gaddr room, gaddr work, gaddr grid, bool shown) {
    const bool wanted = cursor_customizable(g, grid);
    if (wanted && !shown) g_call2(g, kShowPrompts, room, kPromptsCustomize);
    if (!wanted && shown) g_call2(g, kHidePrompts, room, 0u);
    g_st32(g, work + kRoomPromptsShown, wanted ? kPromptsCustomize : kPromptsNone);
}

void refuse_triangle(Guest *g, gaddr work) {
    const gaddr grid = g_ld32(g, work + kRoomGrid);
    const gaddr grid_work = grid != 0u ? g_ld32(g, grid + kGridWork) : 0u;
    if (grid_work == 0u) return;
    const std::uint32_t pressed = g_ld32(g, kLogicalPad0Pressed);
    if ((pressed & rules::kLogicalTriangle) == 0u) return;
    if (!rules::grid_takes_triangle(g_ld32(g, grid_work + kGridState), g_ld32s(g, grid_work + kGridBusy),
                                    g_ld32(g, grid_work + kGridFlags)))
        return;
    if (cursor_customizable(g, grid)) return;
    g_st32(g, kLogicalPad0Pressed, pressed & ~rules::kLogicalTriangle);
    g_call1(g, kMenuSound, kSoundRefused);
    std::cout << "[versus] no Customize for the character under player 2's cursor ("
              << static_cast<std::int32_t>(g_call1(g, kGridCharacter, grid)) << ")" << std::endl;
}

void start_player2_customize(Guest *g, gaddr work, gaddr grid) {
    const std::uint32_t character = g_call1(g, kGridCharacter, grid);
    g_st32(g, work + kRoomOpponent, character);
    g_st32(g, work + kRoomCustomized, character);
    g_call1(g, kCustomizeSettingsInit, work + kRoomCustomizeSettings);
    g_st32(g, work + kRoomFlags, g_ld32(g, work + kRoomFlags) | rules::kFlagCustomize | rules::kFlagFading);
    g_st32(g, work + kRoomBackTo, rules::kBackToPlayer2Pick);
    GuestRegs fade{};
    fade.f[0] = static_cast<float>(static_cast<std::int32_t>(g_call1(g, kFadeFrames, 0u)));
    fade.a[0] = 3u;
    fade.a[1] = kFadeDone;
    fade.a[2] = 0u;
    fade.a[3] = 1u;
    fade.a[4] = 2u;
    g->call(g, kFadeStart, &fade);
    const gaddr colour = g_push(g, 16u);
    for (std::uint32_t i = 0; i < 4u; ++i) g_stf(g, colour + 4u * i, 255.0f);
    g_call2(g, kFadeColour, colour, 2u);
    g_pop(g, 16u);
    state().trip = rules::Trip{rules::Trip::From::Pick};
    std::cout << "[versus] player 2 customises character " << static_cast<std::int32_t>(character)
              << " (from their pick)" << std::endl;
}

void run_player2_pick(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr room = in.gpr[4];
    const gaddr work = g_ld32(g, room + kRoomWork);
    const std::uint32_t flags = work != 0u ? g_ld32(g, work + kRoomFlags) : rules::kFlagFading;
    const bool on = active() && player2_customize();
    bool shown = false;
    if (on && (flags & rules::kFlagFading) == 0u) {
        refuse_triangle(g, work);
        shown = g_ld32(g, work + kRoomPromptsShown) == kPromptsCustomize;
        if (shown) g_st32(g, work + kRoomPromptsShown, kPromptsNone);
    }
    GuestRegs regs = arguments(in);
    decomp::call_original(g, kPlayer2PickUpdate, regs);
    if (!on || work == 0u || (flags & rules::kFlagFading) != 0u) return;
    const gaddr grid = g_ld32(g, work + kRoomGrid);
    const gaddr grid_work = grid != 0u ? g_ld32(g, grid + kGridWork) : 0u;
    if (grid_work == 0u) return;
    const std::uint32_t after = g_ld32(g, work + kRoomState);
    if (rules::pick_customize(flags, after, g_ld32(g, grid_work + kGridResult))) {
        start_player2_customize(g, work, grid);
        return;
    }
    if (after == rules::kPlayer2PickUpdate && (g_ld32(g, work + kRoomFlags) & rules::kFlagFading) == 0u)
        show_player2_prompts(g, room, work, grid, shown);
}

// The game's own update alone, for the differential test (it runs both in a
// copy of the game, where player 2's Customize must not start).
void check_player2_pick(Guest *g, const decomp::CallState &in, GuestRegs &) {
    GuestRegs regs = arguments(in);
    decomp::call_original(g, kPlayer2PickUpdate, regs);
}

#if defined(PORTABLEKIT_HAS_RENDERER)
// The notice over a paused battle whose controller went away: a band in the
// middle of the game's picture, in the PSP's pixels scaled to where the
// picture is.
// Also "Connect a second controller", when the row is chosen without one.
bool notice_visible() { return state().lost != 0u || state().ask_reads != 0u; }

void notice_draw() {
    const std::uint32_t lost = state().lost;
    const portablekit::ui::GamePicture picture = portablekit::ui::game_picture();
    const float sx = picture.width / 480.0f;
    const float sy = picture.height / 272.0f;
    const auto at = [&](float x, float y) { return ImVec2(picture.left + x * sx, picture.top + y * sy); };
    ImDrawList *draw = ImGui::GetForegroundDrawList();
    ImFont *font = portablekit::ui::game_text_font();
    const char *title = lost == 0u                              ? "Connect a second controller"
                        : lost == (kLostPlayer1 | kLostPlayer2) ? "Controllers 1 and 2 disconnected"
                        : (lost & kLostPlayer1) != 0u           ? "Controller 1 disconnected"
                                                                : "Controller 2 disconnected";
    // Player 1 can go on on the keyboard unless it is player 2's.
    const bool keyboard2 = ext::second_pad().keyboard;
    const char *hint = lost == 0u                    ? "Or give Player 2 the keyboard in Controls"
                       : (lost & kLostPlayer2) != 0u ? "Connect it again to go on"
                       : keyboard2                   ? "Connect it again to go on"
                                                     : "Connect it again, or play on the keyboard";
    // Under the pause menu's rows (the game's pause, which the loss opened).
    const float top = 150.0f;
    draw->AddRectFilled(at(90.0f, top), at(390.0f, top + 42.0f), IM_COL32(10, 14, 40, 220), 4.0f * sy);
    draw->AddRect(at(90.0f, top), at(390.0f, top + 42.0f), IM_COL32(255, 196, 64, 255), 4.0f * sy, 0, 1.2f * sy);
    const auto text = [&](float size, float y, ImU32 colour, const char *line) {
        const ImVec2 extent = font->CalcTextSizeA(size * sy, 1.0e9f, 0.0f, line);
        draw->AddText(font, size * sy, ImVec2(at(240.0f, y).x - extent.x * 0.5f, at(240.0f, y).y - extent.y * 0.5f),
                      colour, line);
    };
    text(11.0f, top + 14.0f, IM_COL32(255, 255, 255, 255), title);
    text(8.0f, top + 30.0f, IM_COL32(255, 220, 140, 255), hint);
}

// In a versus battle while player 1's fighter is played (not in the intro)
// and the frame is split (not the VS screen before the battle, where the
// fighters are already played): "1P" and "2P" over the portraits (the stock
// HUD names neither player; the word under Lightning's portrait is her
// paradigm). Once a fighter has fallen, the winner instead.
bool hud_visible() {
    const State &s = state();
    return active() && ((s.fighting && split_now(s)) || s.fallen != 0u || s.party_winner != 0u);
}

void hud_draw() {
    const portablekit::ui::GamePicture picture = portablekit::ui::game_picture();
    const float sx = picture.width / 480.0f;
    const float sy = picture.height / 272.0f;
    const auto at = [&](float x, float y) { return ImVec2(picture.left + x * sx, picture.top + y * sy); };
    ImDrawList *draw = ImGui::GetForegroundDrawList();
    ImFont *font = portablekit::ui::game_text_font();
    const auto centred = [&](float size, float x, float y, ImU32 colour, const char *line) {
        const ImVec2 extent = font->CalcTextSizeA(size * sy, 1.0e9f, 0.0f, line);
        draw->AddText(font, size * sy, ImVec2(at(x, y).x - extent.x * 0.5f, at(x, y).y - extent.y * 0.5f), colour, line);
    };
    const struct {
        float x;
        const char *label;
        ImU32 colour;
    } tags[2] = {{38.0f, "1P", IM_COL32(70, 130, 255, 235)}, {442.0f, "2P", IM_COL32(235, 70, 70, 235)}};
    for (const auto &tag : tags) {
        if (state().fallen != 0u || !state().fighting) break;
        draw->AddRectFilled(at(tag.x - 11.0f, 190.0f), at(tag.x + 11.0f, 201.0f), tag.colour, 3.0f * sy);
        centred(8.5f, tag.x, 195.5f, IM_COL32(255, 255, 255, 255), tag.label);
    }
    // Once a fighter has fallen: who won, over the game's own ending (player
    // 1's results, or player 1's "Return to Battle Setup / Rematch").
    // Player vs Player (Party): after the last fight, over the game's final
    // results (worded from player 1's side), who won the party battle, under
    // the score and the line-ups.
    if (const std::uint32_t winner = state().party_winner; winner != 0u && state().fallen == 0u) {
        const char *line = winner == 3u ? "Draw" : winner == 1u ? "Player 1 wins" : "Player 2 wins";
        draw->AddRectFilled(at(150.0f, 150.0f), at(330.0f, 178.0f), IM_COL32(10, 14, 40, 215), 4.0f * sy);
        draw->AddRect(at(150.0f, 150.0f), at(330.0f, 178.0f), IM_COL32(255, 196, 64, 255), 4.0f * sy, 0, 1.2f * sy);
        centred(15.0f, 240.0f, 164.0f, IM_COL32(255, 222, 130, 255), line);
        return;
    }
    const std::uint32_t fallen = state().fallen;
    if (fallen != 0u) {
        const char *line = fallen == 3u ? "Draw" : fallen == 2u ? "Player 1 wins" : "Player 2 wins";
        draw->AddRectFilled(at(150.0f, 18.0f), at(330.0f, 46.0f), IM_COL32(10, 14, 40, 215), 4.0f * sy);
        draw->AddRect(at(150.0f, 18.0f), at(330.0f, 46.0f), IM_COL32(255, 196, 64, 255), 4.0f * sy, 0, 1.2f * sy);
        centred(15.0f, 240.0f, 32.0f, IM_COL32(255, 222, 130, 255), line);
    }
}

// Player 2's pick: the game has no header for it (its grid headers are
// "Player Select", "CPU Select" and "Customization"), so a red "Player 2
// Select" in the shape of the game's own is drawn over its "Player Select",
// faded in as the game's slides in.
bool pick_header_visible() { return active() && state().picking; }

// A red plate in the shape of the game's headers (pointed ends), in the PSP's
// pixels, faded in from `since` as the game's own slide in.
void draw_player2_plate(float left, float right, float top, float bottom, float text_size, const char *line,
                        std::chrono::steady_clock::time_point since) {
    const portablekit::ui::GamePicture picture = portablekit::ui::game_picture();
    const float sx = picture.width / 480.0f;
    const float sy = picture.height / 272.0f;
    const auto at = [&](float x, float y) { return ImVec2(picture.left + x * sx, picture.top + y * sy); };
    const float shown = std::chrono::duration<float>(std::chrono::steady_clock::now() - since).count();
    const float alpha = std::clamp((shown - 0.1f) / 0.25f, 0.0f, 1.0f);
    if (alpha <= 0.0f) return;
    const auto colour = [&](int r, int g, int b, float a) {
        return IM_COL32(r, g, b, static_cast<int>(a * alpha));
    };
    ImDrawList *draw = ImGui::GetForegroundDrawList();
    ImFont *font = portablekit::ui::game_text_font();
    const float mid = (top + bottom) * 0.5f, point = (bottom - top) * 0.32f;
    const ImVec2 outline[6] = {at(left, mid),           at(left + point, top),    at(right - point, top),
                               at(right, mid),          at(right - point, bottom), at(left + point, bottom)};
    draw->AddConvexPolyFilled(outline, 6, colour(150, 26, 44, 255.0f));
    const float band = (bottom - top) * 0.24f;
    const ImVec2 upper[4] = {at(left + point * 0.5f, mid - band), at(left + point, top + 1.5f),
                             at(right - point, top + 1.5f), at(right - point * 0.5f, mid - band)};
    draw->AddConvexPolyFilled(upper, 4, colour(222, 74, 84, 200.0f));
    draw->AddPolyline(outline, 6, colour(236, 232, 244, 255.0f), ImDrawFlags_Closed, 1.3f * sy);
    const float size = text_size * sy;
    const ImVec2 extent = font->CalcTextSizeA(size, 1.0e9f, 0.0f, line);
    const ImVec2 centre = at((left + right) * 0.5f, mid);
    const ImVec2 origin(centre.x - extent.x * 0.5f, centre.y - extent.y * 0.5f);
    draw->AddText(font, size, ImVec2(origin.x + 0.8f * sx, origin.y + 0.8f * sy), colour(60, 0, 12, 200.0f), line);
    draw->AddText(font, size, origin, colour(255, 255, 255, 255.0f), line);
}

void pick_header_draw() {
    // The game's plate: x 15.5 to 135, y 30.5 to 48.5, pointed ends; this one
    // is longer for the longer words.
    draw_player2_plate(14.5f, 150.0f, 30.0f, 49.0f, 12.5f, "Player 2 Select", state().picking_time);
}

// Player 2's Customize: the game's screen names no player, so "Player 2" in a
// plate of the same kind beside its title, between it and the button
// prompts.
bool customize_header_visible() { return active() && state().customizing; }

void customize_header_draw() {
    draw_player2_plate(156.0f, 226.0f, 7.0f, 23.0f, 11.0f, "Player 2", state().customizing_time);
}
#endif

struct Install {
    Install() {
        if (!enabled()) return;
        static const decomp::Override overrides[] = {
            {kSetAiMode, "versus_set_ai_mode", decomp::Module::BattleOverlay, decomp::Returns::Integer,
             &run_set_ai_mode, true, nullptr, &check_set_ai_mode},
            {kGiveRewards, "versus_give_rewards", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
             &run_give_rewards},
            {kListRewards, "versus_list_rewards", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
             &run_list_rewards},
            {kBuildCpuSide, "versus_build_cpu_side", decomp::Module::Main, decomp::Returns::Nothing,
             &run_build_cpu_side},
            {kPlayer2PickUpdate, "versus_player2_pick", decomp::Module::MenuOverlay, decomp::Returns::Nothing,
             &run_player2_pick, true, nullptr, &check_player2_pick},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
    }
} install_overrides;

constexpr const char *kVersusKey = "game.local_versus";
constexpr const char *kFovKey = "game.versus_fov";

int clamp_fov(int percent) { return std::clamp(percent, kSplitFovMin, kSplitFovMax); }

// The split halves' field of view, in percent: D012_VERSUS_FOV, else
// settings.ini's game.versus_fov, else 100.
std::atomic<int> &split_fov() {
    static std::atomic<int> value = [] {
        if (const char *text = std::getenv("D012_VERSUS_FOV"); text != nullptr && *text != '\0')
            return clamp_fov(std::atoi(text));
        try {
            const std::filesystem::path dir = portablekit::install::user_data_directory();
            const portablekit::install::SettingsEntries entries = portablekit::install::read_settings_file(dir);
            if (const auto found = entries.find(kFovKey); found != entries.end() && !found->second.empty())
                return clamp_fov(std::atoi(found->second.c_str()));
        } catch (const std::exception &e) {
            std::cerr << "[versus] settings.ini not read for " << kFovKey << ": " << e.what() << "\n";
        }
        return 100;
    }();
    return value;
}

#if defined(PORTABLEKIT_HAS_RENDERER)
// The menu's Video page: the split halves' field of view, kept in
// settings.ini as PortableKit keeps its own (it leaves this key alone).
void menu_rows() {
    namespace ui = portablekit::ui;
    ui::section("Local versus");
    int percent = split_fov().load();
    ui::RowOptions options;
    options.description = "How much each half of a two-player split screen shows: 100% is the game's own view. "
                          "Higher sees more around both fighters, and draws them smaller.";
    if (const char *text = std::getenv("D012_VERSUS_FOV"); text != nullptr && *text != '\0') {
        options.disabled = true;
        options.note = "Set by D012_VERSUS_FOV";
    }
    if (!ui::slider_row("Split-screen field of view", percent, kSplitFovMin, kSplitFovMax, 5, "%d%%", options)) return;
    split_fov().store(clamp_fov(percent));
    try {
        const std::filesystem::path dir = portablekit::install::user_data_directory();
        portablekit::install::SettingsEntries entries = portablekit::install::read_settings_file(dir);
        entries[kFovKey] = std::to_string(split_fov().load());
        portablekit::install::write_settings_file(dir, entries);
    } catch (const std::exception &e) {
        std::cerr << "[versus] settings.ini not written for " << kFovKey << ": " << e.what() << "\n";
    }
}
#endif

} // namespace

int split_fov_percent() { return split_fov().load(); }

bool enabled() {
    static const bool value = [] {
        if (const char *text = std::getenv("D012_LOCAL_VERSUS"); text != nullptr) return std::strcmp(text, "0") != 0;
        // settings.ini's game.local_versus, which PortableKit's own settings
        // leave alone (they rewrite only their keys). Written as 1 when the
        // file lacks it, so that a player finds it.
        try {
            const std::filesystem::path dir = portablekit::install::user_data_directory();
            if (!std::filesystem::exists(dir / portablekit::install::kSettingsFile)) return true;
            portablekit::install::SettingsEntries entries = portablekit::install::read_settings_file(dir);
            const auto found = entries.find(kVersusKey);
            if (found == entries.end()) {
                entries[kVersusKey] = "1";
                portablekit::install::write_settings_file(dir, entries);
                return true;
            }
            const std::string &text = found->second;
            if (text == "0" || text == "off" || text == "false") {
                std::cout << "[versus] local versus off (settings.ini " << kVersusKey << "=" << text << ")" << std::endl;
                return false;
            }
        } catch (const std::exception &e) {
            std::cerr << "[versus] settings.ini not read for " << kVersusKey << ": " << e.what() << "\n";
        }
        return true;
    }();
    return value;
}

bool forced() {
    static const bool value = [] {
        const char *text = std::getenv("D012_LOCAL_VERSUS");
        return text != nullptr && std::strcmp(text, "1") == 0;
    }();
    return value;
}

bool active() { return enabled() && (forced() || state().chosen); }

bool party() { return active() && state().party; }

bool party_setup() { return party() && state().party_setup; }

void set_chosen(bool chosen) {
    State &s = state();
    if (s.chosen != chosen) std::cout << "[versus] " << (chosen ? "versus chosen" : "versus off") << std::endl;
    s.chosen = chosen;
}

void note_start_menu() { state().start_menu = true; }

bool second_pad_connected() { return state().connected; }

void ask_for_second_pad() { state().ask_reads = 90u; }

RightStick player2_right_stick() {
    const State &s = state();
    if (!s.pad.connected) return {};
    return {s.pad.right_x, s.pad.right_y};
}

void note_camera_live() { state().camera_live_read = state().reads; }

void note_split_frame(bool split) {
    State &s = state();
    s.split = split;
    if (split) s.split_time = std::chrono::steady_clock::now();
}

bool split_recently() {
    const State &s = state();
    return active() && split_now(s);
}

// The game's pad read (one a frame): player 2's pad to the game, as the
// header says.
void read_pads(const portablekit::HleFunction &previous, psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
    const std::uint32_t buffer = ctx.gpr[4];
    const std::uint32_t count = std::clamp<std::uint32_t>(ctx.gpr[5], 1u, 64u);
    State &s = state();
    psprecomp::GuestMemory &memory = rt.memory();
    const bool battle = battle_resident(memory);
    // Player 2's gamepad is looked for while versus is on, and on the
    // start menu so that the row knows whether there is one; otherwise a
    // second gamepad stays a spare for player 1. (The renderer exists by
    // the game's first pad read.)
    if (battle && !active()) s.start_menu = false;
    const bool want = active() || s.start_menu;
    if (want != s.wanted) {
        s.wanted = want;
        ext::want_second_pad(want);
    }
    // On the start menu a gamepad held for player 2 moves to player 1 when
    // player 1's goes; in versus it stays player 2's.
    if (active() != s.playing) {
        s.playing = active();
        ext::second_player_playing(s.playing);
    }
    previous(rt, ctx);
    ++s.reads;
    if (s.ask_reads != 0u) --s.ask_reads;
    // Player 2's pad from the frame's input record (d012_input.cpp), which is
    // the second gamepad unless a replay or netplay supplies it.
    const input::Pad &p2 = input::current().player2;
    s.pad = ext::SecondPad{p2.connected, p2.buttons, p2.extra, p2.lx, p2.ly, p2.rx, p2.ry};
    if (!battle) s.lost = 0u;
    if (!battle) set_opponent_header(memory);
    if (enabled()) repair_looks(memory, s, battle);
    trace_party(memory, s, battle);
    {
        const bool party = active() && memory.load32(kDestination) == kDestinationParty;
        if (party != s.party) {
            s.party = party;
            std::cout << "[versus] " << (party ? "Player vs Player (Party)" : "not a party battle") << std::endl;
        }
    }
    if (!active()) {
        save_before().clear();
        s.connected = s.pad.connected;
        s.picking = false;
        s.fighting = false;
        s.split = false;
        s.fallen = 0u;
        s.trip = {};
        s.player2_menu = false;
        s.setup_press = false;
        s.setup_wait = 0u;
        s.customizing = false;
        return;
    }
    read_fighting(memory, s, battle);
    {
        const Room room = battle ? Room{} : read_room(memory);
        party_wording(memory, s, room, battle);
        party_tally(memory, s, room, battle);
        s.party_setup = s.party && room.present && (room.state == 28u || room.state == 29u);
    }
    guard_save(memory, battle);
    if (s.pad.connected != s.connected) {
        s.connected = s.pad.connected;
        std::cout << "[versus] player 2's pad " << (s.connected ? "connected" : "disconnected") << std::endl;
        // Lost in battle: the battle pauses as player 2 paused it, so
        // the pause menu waits for player 2's pad to come back. Not when
        // player 1's loss has paused it already: START would resume.
        if (!s.connected && battle) {
            if (s.lost == 0u) s.pause_reads = 1u;
            s.lost |= kLostPlayer2;
            std::cout << "[versus] the battle pauses until player 2's pad is back" << std::endl;
        }
    }
    if (s.connected) s.lost &= ~kLostPlayer2;
    // A party battle's next fight starting while a controller is gone (lost
    // between fights, at the menu or the VS screen, where START does nothing):
    // it pauses as a loss in a fight does, once the fight is under way, as the
    // lost player's pause (player 2's if both).
    constexpr std::uint32_t kPauseDelayReads = 45u;
    if (s.fighting && !s.was_fighting && s.party && !s.connected) s.lost |= kLostPlayer2;
    if (s.fighting && !s.was_fighting && s.party && s.lost != 0u) {
        s.pause_in = kPauseDelayReads;
        std::cout << "[versus] the next fight pauses until the controller is back" << std::endl;
    }
    if (s.pause_in != 0u && --s.pause_in == 0u && s.fighting && s.lost != 0u) {
        if ((s.lost & kLostPlayer2) != 0u)
            s.pause_reads = 1u;
        else
            s.pause1_reads = 1u;
    }
    if (!s.fighting) s.pause_in = 0u;
    s.was_fighting = s.fighting;
    const bool first = input::current().player1.connected;
    if (first != s.first_connected) {
        s.first_connected = first;
        if (!first && battle) {
            if (s.lost == 0u) s.pause1_reads = 1u;
            s.lost |= kLostPlayer1;
            std::cout << "[versus] player 1's gamepad disconnected: the battle pauses" << std::endl;
        }
    }
    if (first) s.lost &= ~kLostPlayer1;
    // "Or play on the keyboard": with no gamepad, player 1's pad is the
    // keyboard and mouse, so any press or stick on it takes the notice away
    // (also when Controls > Player 1 is set to the keyboard in a battle). The
    // pause stays until player 1 presses Start, as when a gamepad comes back.
    // Not while the keyboard is player 2's.
    if (!first && (s.lost & kLostPlayer1) != 0u && s.pause1_reads == 0u && !ext::second_pad().keyboard) {
        const input::Pad &p1 = input::current().player1;
        const auto off_centre = [](std::uint8_t v) { return v < 0x50u || v > 0xB0u; };
        if (p1.buttons != 0u || off_centre(p1.lx) || off_centre(p1.ly)) {
            s.lost &= ~kLostPlayer1;
            std::cout << "[versus] player 1 goes on on the keyboard" << std::endl;
        }
    }
    if (s.pause1_reads != 0u && buffer != 0u) {
        --s.pause1_reads;
        for (std::uint32_t i = 0; i < count; ++i)
            memory.store32(buffer + i * 16u + 4u, memory.load32(buffer + i * 16u + 4u) | kStart);
    }
    // D012_TEST_REVENGE=1, a test switch: R + square on player 1's pad
    // for the read in which player 1's Bravery (status block +0x14)
    // drops, the moment a hit lands, which is when EX Mode is EX Revenge.
    static const bool revenge = std::getenv("D012_TEST_REVENGE") != nullptr;
    if (revenge && battle && buffer != 0u) {
        static std::uint32_t last = 0;
        const std::uint32_t field = memory.load32(kBattleManager);
        const std::uint32_t player = user_ram(field) ? memory.load32(field + 0x38u) : 0u;
        const std::uint32_t status = user_ram(player) ? memory.load32(player + 0x52Cu) : 0u;
        const std::uint32_t brave = user_ram(status) ? memory.load16(status + 0x14u) : 0u;
        if (brave < last)
            for (std::uint32_t i = 0; i < count; ++i)
                memory.store32(buffer + i * 16u + 4u, memory.load32(buffer + i * 16u + 4u) | 0x0200u | 0x8000u);
        last = brave;
    }
    feed_raw_pad1(memory, s);
    trace_fighters(memory, s);
    const Room room = battle ? Room{} : read_room(memory);
    const bool picking = picking_opponent(room);
    if (picking != s.picking) {
        s.picking = picking;
        if (picking) s.picking_time = std::chrono::steady_clock::now();
    }
    // Player 2's triangle given to the setup screen at the last read: if the
    // setup chose its Customize from it, the room takes it a frame later
    // (menu 0x08A9E5E0 sets it going to Customize with player 1's
    // character), and Customize is then given player 2's.
    if (s.setup_press) {
        s.setup_press = false;
        if (rules::setup_chose_customize(s.setup_result, setup_result(memory, room)))
            s.setup_wait = rules::kSetupWaitReads;
    }
    if (s.setup_wait != 0u) {
        --s.setup_wait;
        if (!room.present || room.state != rules::kSetupUpdate) {
            s.setup_wait = 0u;
        } else if (rules::setup_took_customize(s.setup_flags, room.flags, memory.load32(room.work + kRoomBackTo))) {
            s.setup_wait = 0u;
            const std::uint32_t character = memory.load32(room.work + kRoomOpponent);
            memory.store32(room.work + kRoomCustomized, character);
            s.trip = rules::Trip{rules::Trip::From::Setup};
            std::cout << "[versus] player 2 customises character " << static_cast<std::int32_t>(character)
                      << " (from the setup screen)" << std::endl;
        }
    }
    const bool trip = s.trip.active();
    rules::step(s.trip, room.present, room.state, room.next);
    if (trip && !s.trip.active()) std::cout << "[versus] player 2's Customize closed" << std::endl;
    const bool customizing = s.trip.active() && room.state == rules::kCustomizeOpen;
    if (customizing && !s.customizing) s.customizing_time = std::chrono::steady_clock::now();
    s.customizing = customizing;
    const bool player2_menu = room.present && rules::room_pad(room.state, room.next, s.trip) == rules::Pad::Player2;
    if (player2_menu != s.player2_menu) {
        s.player2_menu = player2_menu;
        std::cout << "[versus] "
                  << (!player2_menu ? "player 1's pad again"
                      : picking     ? "player 2 picks the opponent"
                                    : "player 2's pad in Customize")
                  << std::endl;
    }
    const std::uint32_t p2_buttons = s.pad.connected ? s.pad.buttons : 0u;
    const std::uint32_t p2_before = s.p2_before;
    s.p2_before = p2_buttons;
    if (buffer == 0u) return;
    if (player2_menu) {
        for (std::uint32_t i = 0; i < count; ++i) {
            const std::uint32_t sample = buffer + i * 16u;
            memory.store32(sample + 4u, p2_buttons);
            memory.store8(sample + 8u, s.pad.connected ? s.pad.analog_x : 0x80u);
            memory.store8(sample + 9u, s.pad.connected ? s.pad.analog_y : 0x80u);
            memory.store8(sample + 10u, s.pad.connected ? s.pad.right_x : 0x80u);
            memory.store8(sample + 11u, s.pad.connected ? s.pad.right_y : 0x80u);
        }
        return;
    }
    // The setup screen is player 1's, but player 2's triangle is the setup's
    // Customize for player 2's character: a press on the game's pad for this
    // read, when player 1 is not pressing it themselves.
    if (!room.present || !player2_customize()) return;
    std::uint32_t p1_buttons = 0;
    for (std::uint32_t i = 0; i < count; ++i) p1_buttons |= memory.load32(buffer + i * 16u + 4u);
    if (!rules::setup_triangle(room.state, room.flags, p2_buttons, p2_before, p1_buttons)) return;
    const std::uint32_t character = memory.load32(room.work + kRoomOpponent);
    if (!customizable_character(character)) {
        std::cout << "[versus] no Customize for player 2's character " << static_cast<std::int32_t>(character)
                  << std::endl;
        GuestRegs regs{};
        regs.a[0] = kSoundRefused;
        try {
            decomp::call_game(rt, ctx, kMenuSound, regs);
        } catch (const std::exception &e) {
            std::cerr << "[versus] the game's menu sound failed: " << e.what() << "\n";
        }
        return;
    }
    for (std::uint32_t i = 0; i < count; ++i)
        memory.store32(buffer + i * 16u + 4u, memory.load32(buffer + i * 16u + 4u) | rules::kTriangle);
    s.setup_press = true;
    s.setup_flags = room.flags;
    s.setup_result = setup_result(memory, room);
}

void install(portablekit::HleRegistrar &hle) {
    static bool installed = false;
    if (installed || !enabled()) return;
    installed = true;
    const portablekit::HleFunction previous = hle.bound_function("sceCtrl", kSceCtrlReadBufferPositive);
    if (!previous) {
        std::cerr << "[versus] sceCtrlReadBufferPositive is not bound; player 2 has no pad\n";
        return;
    }
    std::cout << "[versus] local versus built in" << (forced() ? ", on in every Quick Battle (D012_LOCAL_VERSUS=1)" : "")
              << ": a second gamepad is player 2's in versus" << std::endl;
#if defined(PORTABLEKIT_HAS_RENDERER)
    portablekit::ui::add_game_overlay(&hud_visible, &hud_draw);
    portablekit::ui::add_game_overlay(&notice_visible, &notice_draw);
    portablekit::ui::add_game_overlay(&pick_header_visible, &pick_header_draw);
    portablekit::ui::add_game_overlay(&customize_header_visible, &customize_header_draw);
    portablekit::ui::add_menu_rows(&menu_rows);
#endif
    if (const portablekit::HleFunction saving = hle.bound_function("sceUtility", kSavedataInitStart); saving) {
        hle.bind("sceUtility", kSavedataInitStart, [saving](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
            psprecomp::GuestMemory &memory = rt.memory();
            const std::uint32_t params = ctx.gpr[4];
            const std::uint32_t mode = user_ram(params) ? memory.load32(params + kSavedataMode) : 0xFFFFFFFFu;
            // AUTOSAVE, SAVE, LISTSAVE: the game's image as it was before the battle.
            if (!save_before().empty() && battle_resident(memory) && (mode == 1u || mode == 3u || mode == 5u)) {
                const std::uint32_t buffer = memory.load32(params + kSavedataBuffer);
                // Only a buffer laid out as the image (its header: version 10, the size).
                if (user_ram(buffer) && memory.load32(params + kSavedataBufferSize) >= kSaveImageSize &&
                    memory.load32(buffer) == 10u && memory.load32(buffer + 4u) == kSaveImageSize)
                    put_save_back(memory, buffer, "the battle's save");
                if (const std::uint32_t image = save_image(memory); image != 0u && image != buffer)
                    put_save_back(memory, image, "the save image");
            }
            saving(rt, ctx);
        });
    } else {
        std::cerr << "[versus] sceUtilitySavedataInitStart is not bound; a versus battle's save is not guarded\n";
    }
    hle.bind("sceCtrl", kSceCtrlReadBufferPositive, [previous](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
        read_pads(previous, rt, ctx);
        sync::after_pad_read(rt.memory(), ctx.gpr[4], battle_resident(rt.memory()), active());
        rollback::note_pad_read(sync::reads() - 1u, battle_resident(rt.memory()) && active());
    });
}

} // namespace d012::versus
