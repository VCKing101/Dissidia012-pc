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
//     player 2's pad instead of player 1's.
//   - Player 2's pad lost in battle: START on raw pad 1 for one read, so the
//     game's own pause opens as player 2's (the battle checks START on each
//     fighter's pad, battle 0x08B20458, and the pause menu answers the pad
//     that opened it) and waits for player 2's pad to return. Player 1's
//     gamepad lost the same way: START in the game's own pad for one read.
//     A notice over the battle says which controller to plug in again.
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
#include "d012_decomp_harness.hpp"

#include "hle/hle_common.hpp"
#include "install/user_data.hpp"
#include "portablekit/hle_extension.hpp"

#include "psprecomp/runtime.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "imgui.h"
#include "ui/ui.hpp"
#endif

#include <algorithm>
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
constexpr std::uint32_t kFirstOpponentState = 8u;
constexpr std::uint32_t kLastOpponentState = 11u;

struct State {
    std::uint32_t reads = 0;
    std::uint32_t camera_live_read = 0xFFFF0000u;
    ext::SecondPad pad;
    bool wanted = false;
    bool connected = false;
    bool picking = false;
    bool announced_human = false;
    // Pad reads left with START pressed on raw pad 1: player 2's pad went
    // away in battle, and the game's pause, as player 2's, waits for it.
    std::uint32_t pause_reads = 0;
    // The same for player 1's gamepad, on the game's own pad. A player on the
    // keyboard has no gamepad to lose.
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
    // The battle has reached its ending (phase 8 on): no tags again until the
    // next fight, though the fade to the loading screen is still drawn split.
    bool ended = false;
    bool chosen = false;      // the Battle Mode row was chosen
    bool start_menu = false;  // the start menu is up (player 2's pad looked for)
    // Pad reads left for the notice "Connect a second controller".
    std::uint32_t ask_reads = 0;
};

constexpr std::uint32_t kLostPlayer1 = 1u;
constexpr std::uint32_t kLostPlayer2 = 2u;

State &state() {
    static State s;
    return s;
}

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

// The character room is on screen and picking the opponent.
bool picking_opponent(psprecomp::GuestMemory &memory) {
    for (std::uint32_t i = 0; i < 3u; ++i)
        if (memory.load32(kRoomStates + 8u * i + 4u) != kRoomStateSignature[i]) return false;
    const std::uint32_t room = memory.load32(kRoomObject);
    if (!user_ram(room)) return false;
    const std::uint32_t work = memory.load32(room + kRoomWork);
    if (!user_ram(work)) return false;
    const std::uint32_t phase = memory.load32(work + kRoomState);
    return phase >= kFirstOpponentState && phase <= kLastOpponentState;
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
// +0x50, 0x1CEAC bytes), which is kept as the fight starts (phase 1 or 2,
// once a battle; a rematch keeps the first) and put back in the buffer the
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
    if (phase != 1u && phase != 2u) return;
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
    std::printf("[versus] read %u p1 (%.2f, %.2f, %.2f) p2 (%.2f, %.2f, %.2f) p2 ai %d pad1 0x%04X %u,%u "
                "eye1 (%.2f, %.2f, %.2f) d %.2f eye2 (%.2f, %.2f, %.2f) d %.2f\n",
                s.reads, f(player + 0x30u), f(player + 0x34u), f(player + 0x38u), f(opponent + 0x30u),
                f(opponent + 0x34u), f(opponent + 0x38u),
                user_ram(ai) ? static_cast<int>(memory.load32(ai + 0x54u)) : -1,
                memory.load32(kRawPad1 + kRawInjectButtons), memory.load8(kRawPad1 + kRawInjectStick),
                memory.load8(kRawPad1 + kRawInjectStick + 1u), e1[0], e1[1], e1[2], e1[3], e2[0], e2[1], e2[2], e2[3]);
    std::fflush(stdout);
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
    // The battle's phase (*(battle 0x08B0CF20) + 8; 0 to 2 the fight, 9 the
    // victory pose, then the results), logged as it changes.
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
    // (A rematch keeps the battle overlay: its fight, phases 1 and 2, clears it.)
    if (phase >= 8u && phase < 32u) s.ended = true;
    if (phase == 1u || phase == 2u) s.ended = false;
    s.fighting = !s.ended && user_ram(ai) && memory.load32(ai + 0x54u) != 0u;
    // Who won, from the victory pose on (phase 9): the flow's winner (+0x20,
    // battle 0x08B09D88). Not before, though a fighter's HP is 0: the fight
    // goes on until the final blow lands (an EX Burst on a fighter at 0 HP).
    if (phase == 0xFFFFFFFFu || phase < 9u || phase >= 32u) return;
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
    const char *hint = lost == 0u                    ? "Player 2 plays on the second gamepad"
                       : (lost & kLostPlayer2) != 0u ? "Connect it again to go on"
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
    return active() && ((s.fighting && split_now(s)) || s.fallen != 0u);
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
        if (state().fallen != 0u) break;
        draw->AddRectFilled(at(tag.x - 11.0f, 190.0f), at(tag.x + 11.0f, 201.0f), tag.colour, 3.0f * sy);
        centred(8.5f, tag.x, 195.5f, IM_COL32(255, 255, 255, 255), tag.label);
    }
    // Once a fighter has fallen: who won, over the game's own ending (player
    // 1's results, or player 1's "Return to Battle Setup / Rematch").
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

void pick_header_draw() {
    const portablekit::ui::GamePicture picture = portablekit::ui::game_picture();
    const float sx = picture.width / 480.0f;
    const float sy = picture.height / 272.0f;
    const auto at = [&](float x, float y) { return ImVec2(picture.left + x * sx, picture.top + y * sy); };
    const float since = std::chrono::duration<float>(std::chrono::steady_clock::now() - state().picking_time).count();
    const float alpha = std::clamp((since - 0.1f) / 0.25f, 0.0f, 1.0f);
    if (alpha <= 0.0f) return;
    const auto colour = [&](int r, int g, int b, float a) {
        return IM_COL32(r, g, b, static_cast<int>(a * alpha));
    };
    ImDrawList *draw = ImGui::GetForegroundDrawList();
    ImFont *font = portablekit::ui::game_text_font();
    // The game's plate: x 15.5 to 135, y 30.5 to 48.5, pointed ends; this one
    // is longer for the longer words.
    const float left = 14.5f, right = 150.0f, top = 30.0f, bottom = 49.0f, mid = (top + bottom) * 0.5f, point = 6.0f;
    const ImVec2 outline[6] = {at(left, mid),           at(left + point, top),    at(right - point, top),
                               at(right, mid),          at(right - point, bottom), at(left + point, bottom)};
    draw->AddConvexPolyFilled(outline, 6, colour(150, 26, 44, 255.0f));
    const ImVec2 upper[4] = {at(left + point * 0.5f, mid - 4.5f), at(left + point, top + 1.5f),
                             at(right - point, top + 1.5f), at(right - point * 0.5f, mid - 4.5f)};
    draw->AddConvexPolyFilled(upper, 4, colour(222, 74, 84, 200.0f));
    draw->AddPolyline(outline, 6, colour(236, 232, 244, 255.0f), ImDrawFlags_Closed, 1.3f * sy);
    const char *line = "Player 2 Select";
    const float size = 12.5f * sy;
    const ImVec2 extent = font->CalcTextSizeA(size, 1.0e9f, 0.0f, line);
    const ImVec2 centre = at((left + right) * 0.5f, mid);
    const ImVec2 origin(centre.x - extent.x * 0.5f, centre.y - extent.y * 0.5f);
    draw->AddText(font, size, ImVec2(origin.x + 0.8f * sx, origin.y + 0.8f * sy), colour(60, 0, 12, 200.0f), line);
    draw->AddText(font, size, origin, colour(255, 255, 255, 255.0f), line);
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
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
    }
} install_overrides;

constexpr const char *kVersusKey = "game.local_versus";

} // namespace

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
        previous(rt, ctx);
        ++s.reads;
        if (s.ask_reads != 0u) --s.ask_reads;
        s.pad = ext::second_pad();
        if (!battle) s.lost = 0u;
        if (!battle) set_opponent_header(memory);
        if (enabled()) repair_looks(memory, s, battle);
        if (!active()) {
            save_before().clear();
            s.connected = s.pad.connected;
            s.picking = false;
            s.fighting = false;
            s.split = false;
            s.fallen = 0u;
            return;
        }
        read_fighting(memory, s, battle);
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
        const bool first = ext::first_pad_connected();
        if (first != s.first_connected) {
            s.first_connected = first;
            if (!first && battle) {
                if (s.lost == 0u) s.pause1_reads = 1u;
                s.lost |= kLostPlayer1;
                std::cout << "[versus] player 1's gamepad disconnected: the battle pauses" << std::endl;
            }
        }
        if (first) s.lost &= ~kLostPlayer1;
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
        const bool picking = picking_opponent(memory);
        if (picking != s.picking) {
            s.picking = picking;
            if (picking) s.picking_time = std::chrono::steady_clock::now();
            std::cout << "[versus] " << (picking ? "player 2 picks the opponent" : "player 1's pad again")
                      << std::endl;
        }
        if (!picking || buffer == 0u) return;
        for (std::uint32_t i = 0; i < count; ++i) {
            const std::uint32_t sample = buffer + i * 16u;
            memory.store32(sample + 4u, s.pad.connected ? s.pad.buttons : 0u);
            memory.store8(sample + 8u, s.pad.connected ? s.pad.analog_x : 0x80u);
            memory.store8(sample + 9u, s.pad.connected ? s.pad.analog_y : 0x80u);
            memory.store8(sample + 10u, s.pad.connected ? s.pad.right_x : 0x80u);
            memory.store8(sample + 11u, s.pad.connected ? s.pad.right_y : 0x80u);
        }
    });
}

} // namespace d012::versus
