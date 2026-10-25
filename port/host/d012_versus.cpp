// Local versus: player 2 on a second controller (docs/port/local-versus.md,
// section 1). On only with D012_LOCAL_VERSUS=1.
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
//     that opened it) and waits for player 2's pad to return.
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
#include "portablekit/hle_extension.hpp"

#include "psprecomp/runtime.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

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
};

State &state() {
    static State s;
    return s;
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
    std::printf("[versus] read %u p1 (%.2f, %.2f, %.2f) p2 (%.2f, %.2f, %.2f) p2 ai %d pad1 0x%04X %u,%u\n",
                s.reads, f(player + 0x30u), f(player + 0x34u), f(player + 0x38u), f(opponent + 0x30u),
                f(opponent + 0x34u), f(opponent + 0x38u),
                user_ram(ai) ? static_cast<int>(memory.load32(ai + 0x54u)) : -1,
                memory.load32(kRawPad1 + kRawInjectButtons), memory.load8(kRawPad1 + kRawInjectStick),
                memory.load8(kRawPad1 + kRawInjectStick + 1u));
    std::fflush(stdout);
}

// battle 0x08B2F2E4, with the Quick Battle set-up's CPU call for slot 1
// asking for the player's mode.
void run_set_ai_mode(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    const gaddr fighter = in.gpr[4];
    std::uint32_t mode = in.gpr[5];
    const std::uint32_t caller = in.gpr[31];
    const gaddr field = g_ld32(g, kBattleManager);
    if (mode == 0u && caller >= kFieldSetup && caller < kFieldSetup + kFieldSetupSize && field != 0u &&
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

struct Install {
    Install() {
        if (!on()) return;
        static const decomp::Override overrides[] = {
            {kSetAiMode, "versus_set_ai_mode", decomp::Module::BattleOverlay, decomp::Returns::Integer,
             &run_set_ai_mode, true, nullptr, &check_set_ai_mode},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
    }
} install_overrides;

} // namespace

bool on() {
    static const bool value = [] {
        const char *text = std::getenv("D012_LOCAL_VERSUS");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    return value;
}

RightStick player2_right_stick() {
    const State &s = state();
    if (!s.pad.connected) return {};
    return {s.pad.right_x, s.pad.right_y};
}

void note_camera_live() { state().camera_live_read = state().reads; }

void install(portablekit::HleRegistrar &hle) {
    static bool installed = false;
    if (installed || !on()) return;
    installed = true;
    const portablekit::HleFunction previous = hle.bound_function("sceCtrl", kSceCtrlReadBufferPositive);
    if (!previous) {
        std::cerr << "[versus] sceCtrlReadBufferPositive is not bound; player 2 has no pad\n";
        return;
    }
    std::cout << "[versus] local versus on: a second gamepad is player 2's" << std::endl;
    hle.bind("sceCtrl", kSceCtrlReadBufferPositive, [previous](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
        const std::uint32_t buffer = ctx.gpr[4];
        const std::uint32_t count = std::clamp<std::uint32_t>(ctx.gpr[5], 1u, 64u);
        State &s = state();
        if (!s.wanted) {
            // The renderer exists by the game's first pad read.
            s.wanted = true;
            ext::want_second_pad(true);
        }
        previous(rt, ctx);
        ++s.reads;
        s.pad = ext::second_pad();
        psprecomp::GuestMemory &memory = rt.memory();
        if (s.pad.connected != s.connected) {
            s.connected = s.pad.connected;
            std::cout << "[versus] player 2's pad " << (s.connected ? "connected" : "disconnected") << std::endl;
            // Lost in battle: the battle pauses as player 2 paused it, so
            // the pause menu waits for player 2's pad to come back.
            if (!s.connected && battle_resident(memory)) {
                s.pause_reads = 1u;
                std::cout << "[versus] the battle pauses until player 2's pad is back" << std::endl;
            }
        }
        feed_raw_pad1(memory, s);
        trace_fighters(memory, s);
        const bool picking = picking_opponent(memory);
        if (picking != s.picking) {
            s.picking = picking;
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
