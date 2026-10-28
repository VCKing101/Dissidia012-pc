// Local versus, the second view (docs/port/local-versus.md, section 2): built
// in unless D012_LOCAL_VERSUS=0, and drawn only in a versus battle.
//
// Each battle frame, after the battle's own draw tasks, one more task in the
// battle's task list draws the scene again from the opponent's camera:
//
//   1. a port marker in the display list (GeState::MarkerSink) has the
//      renderer copy the frame drawn so far, player 1's view, aside;
//   2. the camera manager's view is rebuilt from the opponent's camera slot,
//      the screen cleared, and the battle's draw tasks (filter 0xFFFFFFFF,
//      priorities 58000 to 100200) run again into the same frame buffer;
//   3. the camera manager and the graphics manager's camera are put back,
//      so the next frame and the HUD drawn after this task see player 1's;
//   4. a second marker has the renderer put the middle half of each view side
//      by side back into the frame buffer, player 1 on the left, so the stock
//      HUD, drawn after the battle's task list, lies over both views: player
//      1's gauges over the left one and player 2's over the right.
//
// The two lights that follow the camera are set again for each view
// (camera_lights). Nothing else of section 2.3 is handled yet.
// While a scripted or event camera is in charge (intros, EX Bursts) there is
// no second view.
//
// The task is added from an override of the camera manager's draw-task thunk
// (battle 0x08B3C43C), which runs every battle frame inside the battle's task
// dispatch: the running task (main 0x089D69B0) names the battle's task owner.
//
// MIT licence; see port/README.md.

#include "d012_decomp_harness.hpp"
#include "d012_versus.hpp"

#include "hle/hle_common.hpp"

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace d012::split_view {
namespace {

// main
constexpr std::uint32_t kRunningTask = 0x089D69B0u;      // the task the dispatcher is running
constexpr std::uint32_t kTaskFactory = 0x088823C4u;      // (owner, filter, priority, fn, name) -> task
constexpr std::uint32_t kGraphics = 0x089D5EC0u;         // -> graphics manager
constexpr std::uint32_t kBattleManager = 0x089D6A10u;    // -> FIELD
constexpr std::uint32_t kEffectManager = 0x089D6D80u;    // -> effect manager
constexpr std::uint32_t kGuContext = 0x09AF6830u;        // -> libGu's current list; write cursor at +8
constexpr std::uint32_t kViewSetEye = 0x0885EAA8u;       // (view, vec4 *)
constexpr std::uint32_t kViewSetAt = 0x0885EAC4u;
constexpr std::uint32_t kViewSetUp = 0x0885EAE0u;
constexpr std::uint32_t kViewBuild = 0x0885E860u;        // (view)
constexpr std::uint32_t kProjectionSetFov = 0x0887358Cu; // (f12 fov, projection)
constexpr std::uint32_t kProjectionBuild = 0x08873500u;  // (projection)
constexpr std::uint32_t kEffectCamera = 0x088A5F1Cu;     // (effects, view, projection)
constexpr std::uint32_t kScreenTarget = 0x08866AE0u;     // (graphics): draw buffer, scissor, offset
constexpr std::uint32_t kLoadCamera = 0x08866BA8u;       // (graphics): view and projection to the GE
constexpr std::uint32_t kGuClear = 0x089A0AC4u;          // sceGuClear(flags)
constexpr std::uint32_t kGuLight = 0x089A0D48u;          // sceGuLight(light, type, components, vec3 *)
constexpr std::uint32_t kWorldToScreen = 0x08866E04u;    // (out, in) -> in front of the camera
constexpr std::uint32_t kSetPosition = 0x088D9C60u;      // (layout instance, x, y)
constexpr std::uint32_t kSpriteRelease = 0x088DA060u;    // (layout instance)
// battle: the lock-on cursor, a 0x40-byte part of the camera manager at
// +0x878, set up by battle 0x08B41A54 and given a sprite of the cursor's
// layout (layout 0x222, *0x08B422A8) by battle 0x08B41638 (part +8). The
// camera update runs its update, battle 0x08B41BDC(part, fighter), for the
// on-screen camera's fighter through the camera manager's view and
// projection, which places the sprite through the call at battle 0x08B418C8.
constexpr std::uint32_t kCursorPart = 0x878u;
constexpr std::uint32_t kCursorCaller = 0x08B418C8u;
constexpr std::uint32_t kCursorLayout = 0x08B422A8u;
constexpr std::uint32_t kCursorInit = 0x08B41A54u;       // (part)
constexpr std::uint32_t kCursorAttach = 0x08B41638u;     // (part + 8, layout)
constexpr std::uint32_t kCursorShow = 0x08B41830u;       // (part + 8, shown)
constexpr std::uint32_t kCursorUpdate = 0x08B41BDCu;     // (part, fighter)
constexpr std::uint32_t kCursorRelease = 0x08B41B24u;    // (part): its sprite released
constexpr std::uint32_t kCursorEnable = 0x08B41B40u;     // (part, shown)
// Player 2's cursor part, below the EBOOT as the split task.
constexpr std::uint32_t kPlayer2Cursor = 0x08800400u;
// View 2's eye (a vec4), made there for the view's eye setter.
constexpr std::uint32_t kView2Eye = 0x08800600u;
constexpr std::uint32_t kVec4Copy = 0x089A3868u;         // (out, in)
constexpr std::uint32_t kMatrixApplyProjected = 0x089A4AF8u;  // (out, matrix, vec4)
constexpr std::uint32_t kViewProjection = 0x830u;        // gfx: view x projection
constexpr std::uint32_t kGuEnable = 0x089A0384u;         // sceGuEnable(state)
constexpr std::uint32_t kGuDisable = 0x089A03DCu;        // sceGuDisable(state)
constexpr std::uint32_t kViewEye = 0x0885EA90u;          // (view) -> vec4 *
constexpr std::uint32_t kViewAt = 0x0885EA98u;           // (view) -> vec4 *
constexpr std::uint32_t kVec4Sub = 0x089A38E0u;          // (out, a, b), called as the start task does
constexpr std::uint32_t kAtan2 = 0x089A457Cu;            // (f12, f13) -> f0
constexpr std::uint32_t kSqrt = 0x089A47CCu;             // (f12) -> f0
constexpr std::uint32_t kMatrixIdentity = 0x089A3EBCu;   // (matrix)
constexpr std::uint32_t kMatrixRotateX = 0x089A4180u;    // (f12 angle, out, in)
constexpr std::uint32_t kMatrixRotateY = 0x089A4124u;    // (f12 angle, out, in)
constexpr std::uint32_t kMatrixApply = 0x089A3F40u;      // (out, matrix, vec4)
// battle
constexpr std::uint32_t kCameraManager = 0x08B3D6B8u;    // -> camera manager
constexpr std::uint32_t kCameraDrawThunk = 0x08B3C43Cu;  // OBJ_CAMERA_MANAGER::DrawTask
constexpr std::uint32_t kCameraDraw = 0x08B3C0D8u;       // (manager)
// No code is ever loaded below the EBOOT; the task's function and name live
// here.
constexpr std::uint32_t kSplitTask = 0x08800200u;
// The start of an EX Burst (battle 0x08A3EB70, on the EX Burst object
// *0x08A405E0, from the fighter's EX code at 0x08ACB208): see ex_burst.
constexpr std::uint32_t kExBurstStart = 0x08A3EB70u;
constexpr std::uint32_t kSplitTaskName = 0x08800240u;

constexpr std::uint32_t kDrawFilter = 0xFFFFFFFFu;
constexpr std::int32_t kFirstDrawPriority = 58000;
constexpr std::int32_t kLastDrawPriority = 100200;
constexpr std::int32_t kSplitPriority = 100300;
// GE NOPs carrying port markers (GeState::kMarkerTag): VulkanRenderer::kMarkerSplitLeft
// and kMarkerSplitCompose.
constexpr std::uint32_t kMarkerSplitLeft = 0x00A50001u;
constexpr std::uint32_t kMarkerSplitCompose = 0x00A50002u;

constexpr std::uint32_t kManagerView = 0xC0u + 0x50u;  // view, vectors, matrices and projection
constexpr std::uint32_t kGraphicsCamera = 0x720u;
constexpr std::uint32_t kGraphicsCameraSize = 0x8B0u - 0x720u;

// The overrides go in when the mode is built in; they act only while versus
// is active.
bool on() {
    return versus::enabled();
}

struct State {
    std::uint32_t owner = 0;  // the battle task owner the split task was added to
    std::uint32_t task = 0;
    bool running = false;     // inside the second pass
    std::uint64_t frames = 0;
    // The frame was drawn split: the second view was drawn this frame (set at
    // the end of the split task), and still so when the next frame's update
    // places the HUD, until a frame that is not split.
    bool split_active = false;
    bool cursor2 = false;  // player 2's cursor part is set up, with a sprite
    // Inside the camera update of a frame that will be split
    // (split_frame_expected): the cameras frame for a half.
    bool split_camera = false;
    // An EX Burst (run_ex_burst_start): 1 from its start until either
    // fighter has the battle-event bit, 2 while one has it; 0 otherwise.
    // `ex_burst_since` counts split-task frames in state 1.
    std::uint32_t ex_burst = 0;
    std::uint32_t ex_burst_since = 0;
    // Player 2's fighter made the local one for a battle player 2 won
    // (run_winner_decision).
    bool player2_won = false;
    // The most each view has used of its display list (view2_list).
    std::uint32_t peak1 = 0;
    std::uint32_t peak2 = 0;
};

State &state() {
    static State s;
    return s;
}

// For finding which draw task a second-view problem comes from:
// D012_SPLIT_SKIP=p[,p...] leaves the tasks of those priorities out of the
// second pass, D012_SPLIT_ONLY=lo:hi keeps only that range.
bool second_pass_runs(std::int32_t priority) {
    struct Filter {
        std::vector<std::int32_t> skip;
        std::int32_t low = INT32_MIN, high = INT32_MAX;
    };
    static const Filter filter = [] {
        Filter f;
        if (const char *text = std::getenv("D012_SPLIT_SKIP")) {
            for (const char *p = text; *p != '\0';) {
                char *end = nullptr;
                const long value = std::strtol(p, &end, 10);
                if (end == p) break;
                f.skip.push_back(static_cast<std::int32_t>(value));
                p = *end == ',' ? end + 1 : end;
            }
        }
        if (const char *text = std::getenv("D012_SPLIT_ONLY")) {
            char *end = nullptr;
            f.low = static_cast<std::int32_t>(std::strtol(text, &end, 10));
            if (end != nullptr && *end == ':') f.high = static_cast<std::int32_t>(std::strtol(end + 1, nullptr, 10));
        }
        return f;
    }();
    if (priority < filter.low || priority > filter.high) return false;
    for (std::int32_t p : filter.skip)
        if (p == priority) return false;
    return true;
}

bool task_listed(Guest *g, std::uint32_t owner, std::uint32_t task) {
    for (std::uint32_t t = g_ld32(g, owner + 0x4u); t != 0u; t = g_ld32(g, t + 0x34u))
        if (t == task) return true;
    return false;
}

void add_task(Guest *g) {
    State &s = state();
    const std::uint32_t running = g_ld32(g, kRunningTask);
    if (running == 0u) return;
    const std::uint32_t owner = g_ld32(g, running + 0x24u);
    if (owner == 0u) return;
    if (owner == s.owner && s.task != 0u && task_listed(g, owner, s.task)) return;
    const char name[] = "D012::SplitViewTask";
    std::memcpy(g_ptr(g, kSplitTaskName, sizeof name), name, sizeof name);
    s.task = g_call5(g, kTaskFactory, owner, kDrawFilter, static_cast<std::uint32_t>(kSplitPriority), kSplitTask,
                     kSplitTaskName);
    s.owner = owner;
    s.ex_burst = 0u;
    s.player2_won = false;
    std::cout << "[versus] split-view task added to the battle's task list" << std::endl;
}

// A fighter's "local player" flag (fighter +0x5B0 bit 4, which vtable
// +0x144, battle 0x08B2EF74, tests; battle 0x08B2EF44 sets it on player 1's).
// The game asks it for the one player it was written for: which fighter
// main 0x08889E44 hands out as the player's, whose EX Burst prompts are drawn
// at full strength, whose victory or defeat the ending shows.
void set_local(Guest *g, gaddr fighter, bool local) {
    if (fighter == 0u) return;
    const std::uint8_t flags = g_ld8(g, fighter + 0x5B0u);
    g_st8(g, fighter + 0x5B0u, static_cast<std::uint8_t>(local ? flags | 4u : flags & ~4u));
}

std::int32_t fighter_hp(Guest *g, gaddr fighter) {
    const gaddr status = fighter != 0u ? g_ld32(g, fighter + 0x52Cu) : 0u;
    return status != 0u ? g_ld32s(g, status + 0x0Cu) - g_ld32s(g, status + 0x08u) : 1;
}

// The battle's phase (*(battle 0x08B0CF20) + 8): 0 to 2 the fight, 1 to 3 the
// final blow and its replay, 8 the white flash, 9 the victory pose, 10 the
// pose with the winner's words (the rewards), 12 to 14 the results. The flow
// keeps the winner at +0x20 and the fighter knocked out at +0x24 (battle
// 0x08B09D88). 0xFFFFFFFF without one.
constexpr std::uint32_t kBattleFlow = 0x08B0CF20u;
constexpr std::uint32_t kWinnerDecision = 0x08B09D88u;

// battle 0x089DDD74 (HUD, request): a line for the command guide (the HUD's
// guide at +0xA8C, battle 0x089E6AC0, 0x089EA168: the request's first word
// picks one of its two halves, +0x1C + n*0x84C, and 0x089EA91C adds an
// entry). D012_TRACE_GUIDE=1 logs each request's first words and caller.
constexpr std::uint32_t kGuideRequest = 0x089DDD74u;

void run_guide_request(Guest *g, const decomp::CallState &in, GuestRegs &) {
    static const bool on = [] {
        const char *text = std::getenv("D012_TRACE_GUIDE");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    static std::uint32_t told = 0;
    if (on && told < 2000u && in.gpr[5] != 0u) {
        ++told;
        std::string words;
        for (std::uint32_t i = 0; i < 11u; ++i) {
            char text[16];
            std::snprintf(text, sizeof text, " %X", g_ld32(g, in.gpr[5] + 4u * i));
            words += text;
        }
        std::printf("[guide] split %llu request from %08X:%s\n", static_cast<unsigned long long>(state().frames),
                    in.gpr[31], words.c_str());
        std::fflush(stdout);
    }
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    decomp::call_original(g, kGuideRequest, regs);
}

// Player 2's command guide. The guide's lines (the HUD's guide, above) come
// from five functions that the guide's driver (battle 0x08B158C8) runs each
// frame for the player: battle 0x08B15AE8, 0x08B15F80, 0x08B15D70,
// 0x08B164D8 and 0x08B168E4. Each looks at the player's fighter (main
// 0x08889E44, the first fighter whose local flag is set) and posts its lines
// (main 0x0888ACE4) for the player's slot (FIELD +0x54), the request's first
// word: "× : Chase" (message 8), "○ or □ : Attack" (9), the defender's (10),
// and so on. The guide keeps a half for each slot and lays out the slot of a
// fighter that is not the HUD's player on the right (battle 0x089E968C),
// which is player 2's half of the split screen. So, once a frame in the
// fight, the five run again for player 2: player 2's fighter the local one and
// slot 1 the player's, both put back straight after.
// D012_NO_GUIDE_PLAYER2=1 leaves player 2 without a guide, as before.
constexpr std::uint32_t kGuideLines[] = {0x08B15AE8u, 0x08B15F80u, 0x08B15D70u, 0x08B164D8u, 0x08B168E4u};

void guide_for_player2(Guest *g) {
    static const bool off = [] {
        const char *text = std::getenv("D012_NO_GUIDE_PLAYER2");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    if (off) return;
    const gaddr field = g_ld32(g, kBattleManager);
    const gaddr p1 = field != 0u ? g_ld32(g, field + 0x38u) : 0u;
    const gaddr p2 = field != 0u ? g_ld32(g, field + 0x3Cu) : 0u;
    if (p1 == 0u || p2 == 0u || g_ld32(g, field + 0x7A4u) == 0u || g_ld32(g, field + 0x54u) != 0u) return;
    const std::uint8_t flags1 = g_ld8(g, p1 + 0x5B0u), flags2 = g_ld8(g, p2 + 0x5B0u);
    if ((flags1 & 4u) == 0u || (flags2 & 4u) != 0u) return;  // not the usual: player 1 the local one
    set_local(g, p1, false);
    set_local(g, p2, true);
    g_st32(g, field + 0x54u, 1u);
    for (const std::uint32_t lines : kGuideLines) g_call1(g, lines, 0u);
    g_st32(g, field + 0x54u, 0u);
    g_st8(g, p2 + 0x5B0u, flags2);
    g_st8(g, p1 + 0x5B0u, flags1);
    static bool told = false;
    if (!told) {
        told = true;
        std::cout << "[versus] player 2's command guide" << std::endl;
    }
}

// D012_TRACE_GUIDE: both halves of the command guide and their lines, every
// 10 frames (from the split task, after the guide's own update).
void trace_guide(Guest *g) {
    const gaddr field = g_ld32(g, kBattleManager);
    if (field == 0u || g_ld32(g, field + 0x7A4u) == 0u) return;
    static const bool trace = [] {
        const char *text = std::getenv("D012_TRACE_GUIDE");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    if (trace && state().frames % 10u == 0u) {
        const gaddr guide = g_ld32(g, field + 0x7A4u) + 0xA8Cu + 0x124u;
        for (std::uint32_t slot = 0; slot < 2u; ++slot) {
            const gaddr half = guide + 0x1Cu + slot * 0x84Cu;
            std::uint32_t entries = 0;
            for (gaddr e = g_ld32(g, half + 0x810u); e >= 0x08800000u && e < 0x0A000000u && entries < 64u; e = g_ld32(g, e)) {
                ++entries;
                // The line: its state (+0xC), slot (+0x44), message (+0x48),
                // sprite (+0x10) with its place and flags.
                const gaddr sprite = g_ld32(g, e + 0x10u);
                float at[2] = {};
                const bool ok = sprite >= 0x08800000u && sprite < 0x0A000000u;
                if (ok) std::memcpy(at, g_ptr(g, sprite + 0x10u, 8u), 8u);
                std::printf("[guide]   line %08X state %u slot %u message %u sprite %08X x %.0f y %.0f flags %04X\n", e,
                            g_ld32(g, e + 0xCu), g_ld32(g, e + 0x44u), g_ld32(g, e + 0x48u), sprite, at[0], at[1],
                            ok ? static_cast<std::uint16_t>(g_ld16s(g, sprite + 0x20u)) : 0u);
            }
            gaddr frame = g_ld32(g, half + 0x81Cu + 4u);
            if (frame < 0x08800000u || frame >= 0x0A000000u) frame = 0u;
            float xy[2] = {};
            if (frame != 0u) std::memcpy(xy, g_ptr(g, frame + 0x10u, 8u), 8u);
            std::printf("[guide] split %llu half %u at %08X: on %u state %u suppressed %u %u %u, %u entries, "
                        "frame part %08X x %.0f y %.0f flags %04X\n",
                        static_cast<unsigned long long>(state().frames), slot, half, g_ld8(g, half),
                        g_ld32(g, half + 4u), g_ld8(g, half + 0x82Cu), g_ld8(g, half + 0x82Du),
                        g_ld8(g, half + 0x82Eu), entries, frame, xy[0], xy[1],
                        frame != 0u ? static_cast<std::uint16_t>(g_ld16s(g, frame + 0x20u)) : 0u);
        }
    }
}

// battle 0x089E8EF0 (&setting): the command guide's mode, the player's
// setting (0 unless something overrides it). Each frame the guide's update
// keeps a half quiet when the mode is 2, or when it is 0 and the half is the
// one on the right (battle 0x089EAE18, with 0x089EAC98 and 0x089EAC30,
// reason 1); mode 1 keeps both halves on. In versus that step is answered 1
// for a 0, so player 2's half stays on; every other reader of the mode (how
// many lines a half shows, other HUD pieces) gets the setting as it is.
constexpr std::uint32_t kGuideMode = 0x089E8EF0u;
constexpr std::uint32_t kGuideKeepHalves = 0x089EAE18u;
constexpr std::uint32_t kGuideKeepHalvesEnd = 0x089EAEECu;

void run_guide_mode(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    decomp::call_original(g, kGuideMode, regs);
    out.v0 = regs.v0;
    const std::uint32_t caller = in.gpr[31];
    if (regs.v0 == 0u && caller >= kGuideKeepHalves && caller < kGuideKeepHalvesEnd && versus::active()) out.v0 = 1u;
}

// battle 0x08B158C8: the guide's driver, once a frame in the battle's update
// (through a pointer). The game's run for player 1, then player 2's, so that
// player 2's lines arrive when player 1's do, before the guide's own update
// keeps or drops each line.
constexpr std::uint32_t kGuideDriver = 0x08B158C8u;
std::uint32_t battle_phase(Guest *g);

void run_guide_driver(Guest *g, const decomp::CallState &in, GuestRegs &) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    decomp::call_original(g, kGuideDriver, regs);
    if (!versus::active()) return;
    const std::uint32_t phase = battle_phase(g);
    if (phase <= 2u && !state().player2_won) guide_for_player2(g);
}

// main 0x08886A38 (battle state *0x089D69F8, slot): whether the winner (+0xB4)
// is the fighter of that slot (the slot's pointer at state + slot*4). The
// battle's update (main 0x0888C048) asks it for the local slot (FIELD +0x54)
// at the final blow, and the answer picks the victory pose or the defeat. In
// versus a win of player 2's (the winner is FIELD +0x3C's fighter) is a win
// for the screen: the winner's victory pose, as for player 1.
constexpr std::uint32_t kSlotWon = 0x08886A38u;

void run_slot_won(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    const gaddr battle_state = in.gpr[4];
    const gaddr winner = g_ld32(g, battle_state + 0xB4u);
    bool won = winner == g_ld32(g, g_ld32(g, battle_state + in.gpr[5] * 4u));
    if (!won && versus::active()) {
        const gaddr field = g_ld32(g, kBattleManager);
        const gaddr p2 = field != 0u ? g_ld32(g, field + 0x3Cu) : 0u;
        if (p2 != 0u && winner == p2) {
            won = true;
            std::cout << "[versus] the final blow is player 2's: player 2's victory pose" << std::endl;
        }
    }
    out.v0 = won ? 1u : 0u;
}

std::uint32_t battle_phase(Guest *g) {
    const gaddr flow = g_ld32(g, kBattleFlow);
    return flow != 0u ? g_ld32(g, flow + 8u) : 0xFFFFFFFFu;
}

// battle 0x08B09D88 (flow, ...): at the final blow, the winner and the fighter
// knocked out go to the flow, and whether the winner is the player's fighter
// (main 0x08889E44) picks the victory pose or the player's defeat. In versus
// a win of player 2's is player 2's victory: player 2's fighter is made the
// local one (and player 1's not) for the rest of the battle, so the ending is
// the winner's own, as player 1's is.
void run_winner_decision(Guest *g, const decomp::CallState &in, GuestRegs &) {
    State &s = state();
    if (versus::active() && !s.player2_won) {
        const gaddr field = g_ld32(g, kBattleManager);
        const gaddr p1 = field != 0u ? g_ld32(g, field + 0x38u) : 0u;
        const gaddr p2 = field != 0u ? g_ld32(g, field + 0x3Cu) : 0u;
        if (p1 != 0u && p2 != 0u && fighter_hp(g, p1) <= 0 && fighter_hp(g, p2) > 0) {
            set_local(g, p1, false);
            set_local(g, p2, true);
            s.player2_won = true;
            std::cout << "[versus] player 2 won: the ending is player 2's" << std::endl;
        }
    }
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    decomp::call_original(g, kWinnerDecision, regs);
}

// battle 0x08A3EB70 (EX Burst object, fighter, ...): an EX Burst starts. Its
// stages, the glass crack to the last hit, are the one event the split view
// leaves to one full view (the owner's choice): the second view is left out
// from this call until the battle-event bit it brings (fighter +0x5B1 bit
// 0x80) has come and gone. A chase and other events between the fighters set
// that bit too, and stay split.
void run_ex_burst_start(Guest *g, const decomp::CallState &in, GuestRegs &) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    decomp::call_original(g, kExBurstStart, regs);
    if (!versus::active()) return;
    State &s = state();
    if (s.ex_burst == 0u) {
        static std::uint32_t told = 0;
        if (told++ < 5u) std::cout << "[versus] an EX Burst: one full view until it ends" << std::endl;
    }
    s.ex_burst = std::max(s.ex_burst, 1u);
    s.ex_burst_since = 0u;
}

// main 0x088D9AF4 (sprite, &colour): a HUD sprite's colour (+0x28) set, and
// the sprite brought up to date. An EX Burst fades the prompts of the side
// that is not the player's: the attacker's commands when the attacker is not
// the local fighter, the defender's gauge when it is (battle 0x08A3DDF4,
// 0x08A3DFD0 and the moves' own prompts, 0x08A2FB70 to 0x08A4514C), each by
// setting the sprite's colour again with its alpha down to 0x50. In versus
// both sides are players, so such a fade from that code is left out and both
// players' prompts stay as strong as player 1's.
constexpr std::uint32_t kSpriteColour = 0x088D9AF4u;
constexpr std::uint32_t kExBurstPromptsFirst = 0x08A2F000u;
constexpr std::uint32_t kExBurstPromptsEnd = 0x08A46000u;

void run_sprite_colour(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr sprite = in.gpr[4];
    const std::uint32_t caller = in.gpr[31];
    if (versus::active() && caller >= kExBurstPromptsFirst && caller < kExBurstPromptsEnd && sprite != 0u &&
        in.gpr[5] != 0u) {
        const std::uint32_t wanted = g_ld32(g, in.gpr[5]);
        const std::uint32_t now = g_ld32(g, sprite + 0x28u);
        if ((wanted >> 24u) == 0x50u && (now >> 24u) > 0x50u && (wanted & 0x00FFFFFFu) == (now & 0x00FFFFFFu)) {
            // The fade, left out; each caller logged once.
            static std::vector<std::uint32_t> told;
            if (told.size() < 32u && std::find(told.begin(), told.end(), caller) == told.end()) {
                told.push_back(caller);
                std::printf("[versus] an EX Burst prompt kept at full strength (fade from %08X)\n", caller);
                std::fflush(stdout);
            }
            return;
        }
    }
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    decomp::call_original(g, kSpriteColour, regs);
}


void run_camera_draw_thunk(Guest *g, const decomp::CallState &, GuestRegs &) {
    g_call1(g, kCameraDraw, g_ld32(g, kCameraManager));
    if (!state().running && versus::active()) add_task(g);
}

// The opponent's camera slot in the manager, or -1.
int opponent_slot(Guest *g, std::uint32_t manager) {
    const std::uint32_t field = g_ld32(g, kBattleManager);
    if (field == 0u) return -1;
    // D012_SPLIT_SAME_VIEW=1, a test switch: the second view from player 1's
    // camera too, to tell a second-pass problem from a player 2 one.
    static const bool same = [] {
        const char *text = std::getenv("D012_SPLIT_SAME_VIEW");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    const std::uint32_t fighter = g_ld32(g, field + (same ? 0x38u : 0x3Cu));
    if (fighter == 0u) return -1;
    const std::uint32_t camera = g_ld32(g, fighter + 0x5ACu);
    if (camera == 0u) return -1;
    for (int slot = 0; slot < 8; ++slot)
        if (g_ld32(g, manager + 0x6D0u + 4u * static_cast<std::uint32_t>(slot)) == camera) return slot;
    return -1;
}

void emit(Guest *g, std::uint32_t word) {
    const std::uint32_t context = g_ld32(g, kGuContext);
    const std::uint32_t cursor = g_ld32(g, context + 8u);
    g_st32(g, cursor, word);
    g_st32(g, context + 8u, cursor + 4u);
}

// View 2 in a display list of its own. The frame's list (libGu's context:
// +4 its start, +8 the write cursor, +0xC its size, 0x58400 bytes; the game
// keeps two back to back and takes them in turn) holds one view and the HUD
// with room to spare, but not two views on a heavy stage: on Planet's Core,
// with assists, view 2 alone took more than 200 KiB on a frame whose first
// view had used 118 KiB, ran past the end of the second list and over the
// disc reader that follows it in memory, and the music's next refill crashed.
// So the second pass goes into a list of its own: the frame's list jumps to
// it, the game's writes go to it while the second pass runs, and it jumps
// back. The jumps are the GE's signal jumps (SIGNAL 0x10 with the address's
// high half, END with its low half), which take an absolute address and leave
// the BASE register alone.
//
// Where: the kernel partition, which a game cannot touch on a PSP and which
// PortableKit uses only below 0x08200000 (boot arguments, the interrupt
// stack) and from 0x08400000 (volatile memory). In battle the user partition
// has about 500 KiB free, too little to take this much from.
constexpr std::uint32_t kList2 = 0x08300000u;
constexpr std::uint32_t kList2Size = 0x80000u;
constexpr std::uint32_t kList2Reserve = 0x10000u;  // left free at the end: no draw task starts past it

// D012_SPLIT_SHARED_LIST=1 draws view 2 into the frame's list, as before,
// with the half-list guard.
std::uint32_t view2_list() {
    static const bool shared = [] {
        const char *text = std::getenv("D012_SPLIT_SHARED_LIST");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    return shared ? 0u : kList2;
}

void emit_jump(Guest *g, gaddr at, gaddr target) {
    const std::uint32_t physical = target & 0x0FFFFFFCu;
    g_st32(g, at, 0x0E100000u | (physical >> 16u));
    g_st32(g, at + 4u, 0x0C000000u | (physical & 0xFFFFu));
}

struct ListSwitch {
    gaddr context = 0;
    std::uint32_t start = 0, size = 0;
    gaddr resume = 0;  // where the frame's list goes on after view 2
    gaddr list = 0;    // view 2's list, at the address the game writes through
};

ListSwitch list_to_view2(Guest *g, gaddr list2) {
    ListSwitch sw;
    sw.context = g_ld32(g, kGuContext);
    sw.start = g_ld32(g, sw.context + 4u);
    sw.size = g_ld32(g, sw.context + 12u);
    const gaddr cursor = g_ld32(g, sw.context + 8u);
    // The game writes its list through the uncached mirror; so does view 2.
    sw.list = (list2 & 0x0FFFFFFFu) | (cursor & 0xF0000000u);
    emit_jump(g, cursor, sw.list);
    sw.resume = cursor + 8u;
    g_st32(g, sw.context + 4u, sw.list);
    g_st32(g, sw.context + 8u, sw.list);
    g_st32(g, sw.context + 12u, kList2Size);
    return sw;
}

// Whether view 2's list has room for another draw task.
bool view2_list_has_room(Guest *g, const ListSwitch &sw) {
    return sw.context == 0u || g_ld32(g, sw.context + 8u) - sw.list < kList2Size - kList2Reserve;
}

std::uint32_t list_back(Guest *g, const ListSwitch &sw) {
    const gaddr cursor = g_ld32(g, sw.context + 8u);
    emit_jump(g, cursor, sw.resume);
    g_st32(g, sw.context + 4u, sw.start);
    g_st32(g, sw.context + 8u, sw.resume);
    g_st32(g, sw.context + 12u, sw.size);
    return cursor + 8u - sw.list;
}

// The two lights that follow the camera, as the list start task sets them
// from the graphics manager's camera (main 0x08865AC4..0x08865D30): the
// camera's direction (look-at minus eye) gives a yaw, atan2(x, z), and a
// pitch, atan2(sqrt(x*x + z*z), y); a matrix turned by both takes (1, 0, 0)
// to light 2's vector and (0, 0, 1) to light 3's, both directional. Drawn
// once a frame from player 1's camera, they light player 2's view from the
// wrong side: Order's Sanctuary's water came out white.
float call_float(Guest *g, gaddr fn, float f12, float f13) {
    GuestRegs r;
    std::memset(&r, 0, sizeof r);
    r.f[0] = f12;
    r.f[1] = f13;
    g->call(g, fn, &r);
    return r.f0;
}

void call_angle_matrix(Guest *g, gaddr fn, float angle, gaddr matrix) {
    GuestRegs r;
    std::memset(&r, 0, sizeof r);
    r.f[0] = angle;
    r.a[0] = matrix;
    r.a[1] = matrix;
    g->call(g, fn, &r);
}

void camera_lights(Guest *g, std::uint32_t graphics) {
    const gaddr frame = g_push(g, 0xC0u);
    const gaddr x_axis = frame, z_axis = frame + 0x10u, direction = frame + 0x20u, matrix = frame + 0x30u;
    const gaddr light2 = frame + 0x70u, light3 = frame + 0x80u;
    const float one = 1.0f, zero = 0.0f;
    const float axes[8] = {one, zero, zero, one, zero, zero, one, one};
    for (std::uint32_t i = 0; i < 8u; ++i) g_stf(g, frame + 4u * i, axes[i]);
    const gaddr camera = graphics + kGraphicsCamera;
    const gaddr eye = g_call1(g, kViewEye, camera);
    const gaddr at = g_call1(g, kViewAt, camera);
    g_call3(g, kVec4Sub, direction, eye, at);
    float d[3];
    std::memcpy(d, g_ptr(g, direction, 12u), 12u);
    const float yaw = call_float(g, kAtan2, d[0], d[2]);
    const float flat = call_float(g, kSqrt, d[0] * d[0] + d[2] * d[2], 0.0f);
    const float pitch = call_float(g, kAtan2, flat, d[1]);
    g_call1(g, kMatrixIdentity, matrix);
    call_angle_matrix(g, kMatrixRotateX, pitch, matrix);
    call_angle_matrix(g, kMatrixRotateY, yaw, matrix);
    g_call3(g, kMatrixApply, light2, matrix, x_axis);
    g_call3(g, kMatrixApply, light3, matrix, z_axis);
    g_call4(g, kGuLight, 2u, 0u, 2u, light2);
    g_call4(g, kGuLight, 3u, 0u, 2u, light3);
    g_pop(g, 0xC0u);
}

// The render state the list start task (main 0x0886598C) sets after
// starting the frame's list (0x088659F0..0x08865E24), call for call, from
// the graphics manager's camera: the second view must start from the same
// state as the first, not from what the first left. The first view's last
// passes turn the scissor test off in libGu's context, for one, and with it
// off libGu sends no scissor (0x089A204C), so the second view's half-size
// frame copies were drawn with the full-screen scissor and Order's
// Sanctuary's water, which samples them, came out white.
void frame_start_state(Guest *g, std::uint32_t graphics) {
    const auto enable = [g](std::uint32_t state) { g_call1(g, kGuEnable, state); };
    const auto disable = [g](std::uint32_t state) { g_call1(g, kGuDisable, state); };
    g_call1(g, kScreenTarget, graphics);
    enable(2u);
    g_call3(g, 0x089A123Cu, 1u, 0x80u, 0x80u);
    g_call3(g, 0x089A1270u, 0u, 0u, 2u);
    disable(3u);
    g_call1(g, 0x089A0A90u, g_ld32(g, graphics + 0x8BCu));  // clear colour
    g_call1(g, 0x089A0AB4u, 0u);                           // clear depth
    g_call1(g, 0x089A0AA4u, 0u);                           // clear stencil
    g_call1(g, kGuClear, 0x17u);
    g_call1(g, kLoadCamera, graphics);
    enable(7u);
    // Fog: near and far as floats, the colour as an integer, from +0x8B0.
    const std::uint32_t fog = graphics + 0x8B0u;
    GuestRegs r;
    std::memset(&r, 0, sizeof r);
    r.a[0] = fog;
    g->call(g, 0x08864CB8u, &r);
    const float fog_near = r.f0;
    std::memset(&r, 0, sizeof r);
    r.a[0] = fog;
    g->call(g, 0x08864CC0u, &r);
    const float fog_far = r.f0;
    const std::uint32_t fog_colour = g_call1(g, 0x08864CC8u, fog);
    std::memset(&r, 0, sizeof r);
    r.f[0] = fog_near;
    r.f[1] = fog_far;
    r.a[0] = fog_colour;
    g->call(g, 0x089A1070u, &r);
    enable(10u);
    for (std::uint32_t light = 0xBu; light <= 0xEu; ++light) disable(light);
    g_call1(g, 0x089A0C20u, 0xFFFFFFFFu);
    g_call1(g, 0x0899FFC8u, 1u);
    camera_lights(g, graphics);
    enable(0u);
    enable(1u);
    enable(4u);
    if (g_ld8(g, graphics + 0x2CCFu) != 0u) disable(6u);
    else enable(6u);
    enable(9u);
    enable(5u);
    enable(8u);
    enable(0x10u);
    enable(0x14u);
    disable(0xFu);
    g_call1(g, 0x089A0CF8u, 1u);
    g_call1(g, 0x089A0D20u, 1u);
    g_call3(g, 0x089A113Cu, 6u, 4u, 0xFFu);
    g_call5(g, 0x089A12A4u, 0u, 2u, 3u, 0u, 0u);
    g_call1(g, 0x089A1178u, 7u);
    g_call2(g, 0x089A0DF4u, 7u, 1u);
    g_call_f4(g, 0x089A0E8Cu, 0.0f, 0u, 0u, 0u, 0u);
    g_call2(g, 0x089A0E20u, 0u, 1u);
    g_call2(g, 0x089A0E60u, 0u, 0u);
    g_call2(g, 0x0899FF9Cu, 4u, 4u);
    g_call1(g, 0x089A25C0u, 0u);
}

void popups_to_view2(Guest *g, std::vector<std::pair<gaddr, std::array<std::uint32_t, 2>>> &saved);
bool trace_markers();

// The map's fading of objects close to the camera (docs/port/local-versus.md,
// section 7.5). The map (*0x089D6AC8) keeps 16 fade slots of 10 bytes at
// +0x612 (object id, value 0..100, marked, flags, alpha, timer). The camera
// update marks the objects around its eye (main 0x0889B37C, radius 1, the
// third of every three frames) and on the line from its eye to its fighter
// (main 0x0889ACA8, the first) through main 0x08892FD0, which takes a slot and
// switches the object's model part to fading (main 0x0886BB44, the map
// model's display list patched); the map's update (main 0x08893DAC) ages the
// slots, and frees a slot faded back in, switching its part back. The map's
// draw draws the fading parts at their slots' alpha (main 0x0889245C). It all
// follows the on-screen camera. Player 2 keeps a table of its own, swapped in
// for the second pass with the parts switched to match, aged and marked
// there from view 2's camera by the game's own functions, and swapped out.
constexpr std::uint32_t kMap = 0x089D6AC8u;
constexpr std::uint32_t kMapSlots = 0x612u;
constexpr std::uint32_t kMapSlotsSize = 16u * 10u;
constexpr std::uint32_t kMapModel = 0x74u;
constexpr std::uint32_t kMapGrid = 0x08899300u;          // () -> the map's collision grid
constexpr std::uint32_t kMapPartOf = 0x088985F0u;        // (grid, object) -> model part
constexpr std::uint32_t kMapHasPart = 0x088987E8u;       // (grid, object) -> nonzero when it has one
constexpr std::uint32_t kPartFading = 0x0886BB44u;       // (model, part, on, 1, 1)
constexpr std::uint32_t kMapAgeSlots = 0x08893DACu;      // (map)
constexpr std::uint32_t kMarkNearEye = 0x0889B37Cu;      // (f12 radius, grid, eye)
constexpr std::uint32_t kMarkEyeToFighter = 0x0889ACA8u; // (grid, eye, point)
constexpr std::uint32_t kFighterCheck = 0x08837990u;     // (fighter): 0 when the line is marked

using SlotTable = std::array<std::uint8_t, kMapSlotsSize>;

SlotTable empty_slots() {
    SlotTable t{};
    for (std::uint32_t i = 0; i < 16u; ++i) {
        std::uint8_t *slot = t.data() + 10u * i;
        slot[0] = slot[1] = 0xFFu;  // no object
        slot[6] = 0xFFu;            // alpha
    }
    return t;
}

struct MapFade {
    SlotTable view2 = empty_slots();
    std::uint32_t phase = 0;  // the camera's three-frame cycle, for view 2
    std::uint32_t owner = 0;  // the battle the table belongs to
};

MapFade &map_fade() {
    static MapFade f;
    return f;
}

std::int16_t slot_object(const SlotTable &t, std::uint32_t i) {
    std::int16_t object;
    std::memcpy(&object, t.data() + 10u * i, 2u);
    return object;
}

// Switches the parts of the objects `from` fades and `to` does not off, and
// those `to` fades and `from` does not on, as taking and freeing the slots
// would have.
void switch_parts(Guest *g, gaddr map, const SlotTable &from, const SlotTable &to) {
    const gaddr model = g_ld32(g, map + kMapModel);
    if (model == 0u) return;
    const gaddr grid = g_call4(g, kMapGrid, 0u, 0u, 0u, 0u);
    if (grid == 0u || g_ld32(g, grid) == 0u) return;
    const auto has = [](const SlotTable &t, std::int16_t object) {
        for (std::uint32_t i = 0; i < 16u; ++i)
            if (slot_object(t, i) == object) return true;
        return false;
    };
    const auto flip = [&](const SlotTable &mine, const SlotTable &other, std::uint32_t on) {
        for (std::uint32_t i = 0; i < 16u; ++i) {
            const std::int16_t object = slot_object(mine, i);
            if (object < 0 || has(other, object)) continue;
            const std::uint32_t id = static_cast<std::uint32_t>(static_cast<std::int32_t>(object));
            if (g_call2(g, kMapHasPart, grid, id) == 0u) continue;
            const std::int32_t part = static_cast<std::int32_t>(g_call2(g, kMapPartOf, grid, id));
            if (part < 0) continue;
            g_call5(g, kPartFading, model, static_cast<std::uint32_t>(part), on, 1u, 1u);
        }
    };
    flip(from, to, 0u);
    flip(to, from, 1u);
}

// Before the second pass: player 2's table in, aged and marked from view 2's
// camera. `view1` keeps player 1's table to put back.
bool map_fade_to_view2(Guest *g, gaddr manager, int slot, gaddr fighter, std::uint32_t owner, SlotTable &view1) {
    const gaddr map = g_ld32(g, kMap);
    if (map == 0u || g_ld32(g, map + kMapModel) == 0u) return false;
    MapFade &f = map_fade();
    if (f.owner != owner) {  // a new battle: player 2's table starts empty
        f.view2 = empty_slots();
        f.phase = 0u;
        f.owner = owner;
    }
    std::memcpy(view1.data(), g_ptr(g, map + kMapSlots, kMapSlotsSize), kMapSlotsSize);
    switch_parts(g, map, view1, f.view2);
    std::memcpy(g_ptr(g, map + kMapSlots, kMapSlotsSize), f.view2.data(), kMapSlotsSize);
    g_call1(g, kMapAgeSlots, map);
    const gaddr grid = g_call4(g, kMapGrid, 0u, 0u, 0u, 0u);
    const gaddr eye = manager + static_cast<std::uint32_t>(slot) * 0xB0u + 0x110u;
    if (grid != 0u && fighter != 0u) {
        if (f.phase == 0u && g_call1(g, kFighterCheck, fighter) == 0u) {
            g_call3(g, kMarkEyeToFighter, grid, eye, fighter + 0xC0u);
        } else if (f.phase == 2u) {
            const gaddr frame = g_push(g, 0x10u);
            g_call2(g, kVec4Copy, frame, eye);
            g_call_f4(g, kMarkNearEye, 1.0f, grid, frame, 0u, 0u);
            g_pop(g, 0x10u);
        }
    }
    f.phase = (f.phase + 1u) % 3u;
    return true;
}

// D012_TRACE_MAP_FADE=1: the objects each view's table fades, when they change.
void trace_map_fade(const SlotTable &view1, const SlotTable &view2) {
    static const bool on = [] {
        const char *text = std::getenv("D012_TRACE_MAP_FADE");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    if (!on) return;
    static std::string last;
    std::string line;
    for (const SlotTable *t : {&view1, &view2}) {
        line += t == &view1 ? "view 1:" : " | view 2:";
        for (std::uint32_t i = 0; i < 16u; ++i) {
            const std::int16_t object = slot_object(*t, i);
            if (object >= 0) line += " " + std::to_string(object) + "/" + std::to_string((*t)[10u * i + 6u]);
        }
    }
    if (line == last) return;
    last = line;
    std::printf("[map-fade] split %llu %s\n", static_cast<unsigned long long>(state().frames), line.c_str());
    std::fflush(stdout);
}

// After the second pass: player 2's table kept, player 1's back.
void map_fade_back(Guest *g, const SlotTable &view1) {
    const gaddr map = g_ld32(g, kMap);
    if (map == 0u) return;
    MapFade &f = map_fade();
    std::memcpy(f.view2.data(), g_ptr(g, map + kMapSlots, kMapSlotsSize), kMapSlotsSize);
    trace_map_fade(view1, f.view2);
    switch_parts(g, map, f.view2, view1);
    std::memcpy(g_ptr(g, map + kMapSlots, kMapSlotsSize), view1.data(), kMapSlotsSize);
}

// Fighters close to the camera turn see-through. The camera manager keeps, per
// camera slot, a value, a target and a rate at +0x6F0 (12 bytes each); its
// update (battle 0x08B3CA24) moves them for the on-screen camera (battle
// 0x08B3C1E0: towards 0.5 while the slot's fighter is within its radius + 0.5
// of the camera, else back to 1) when the camera's mode allows (mode vtable
// +0x68), else resets them (battle 0x08B3C1A8), and sets each fighter's fade
// bit (+0x380, bit 0x200000) and alpha (+0x490) from them; the draw tasks
// leave a fighter with the bit to DrawAlphaScreenTask (main 0x088419EC).
// Player 2 keeps values of its own, moved from player 2's camera right after
// the update (the fighters' spheres the step reads hold only then), and
// applied to the fighters for the second pass.
constexpr std::uint32_t kFadeValues = 0x6F0u;
constexpr std::uint32_t kFadeValuesSize = 8u * 12u;
constexpr std::uint32_t kFadeStep = 0x08B3C1E0u;   // (manager, camera)
constexpr std::uint32_t kFadeReset = 0x08B3C1A8u;  // (manager)
constexpr std::uint32_t kCameraUpdateThunk = 0x08B3C41Cu;  // OBJ_CAMERA_MANAGER's update task
constexpr std::uint32_t kCameraUpdate = 0x08B3CA24u;       // (manager)

struct FighterFade {
    std::array<std::uint8_t, kFadeValuesSize> view2{};
    std::uint32_t owner = 0;
};

FighterFade &fighter_fade() {
    static FighterFade f;
    return f;
}

struct FighterFadeSaved {
    std::vector<std::array<std::uint32_t, 3>> fighters;  // fighter, +0x380, +0x490
};

// Player 2's values, a step on from player 2's camera; after the update.
void step_fighter_fade_view2(Guest *g, gaddr manager) {
    State &s = state();
    if (s.owner == 0u || g_ld8(g, manager + 0x8BCu) != 1u) return;
    const int slot = opponent_slot(g, manager);
    if (slot < 0) return;
    const gaddr camera = g_ld32(g, manager + 0x6D0u + 4u * static_cast<std::uint32_t>(slot));
    if (camera == 0u) return;
    FighterFade &f = fighter_fade();
    std::array<std::uint8_t, kFadeValuesSize> view1;
    std::memcpy(view1.data(), g_ptr(g, manager + kFadeValues, kFadeValuesSize), kFadeValuesSize);
    if (f.owner != s.owner) {  // a new battle: player 2's values start as the reset leaves them
        g_call1(g, kFadeReset, manager);
        std::memcpy(f.view2.data(), g_ptr(g, manager + kFadeValues, kFadeValuesSize), kFadeValuesSize);
        f.owner = s.owner;
    }
    std::memcpy(g_ptr(g, manager + kFadeValues, kFadeValuesSize), f.view2.data(), kFadeValuesSize);
    const std::int32_t mode = g_ld32s(g, camera + 0x108u);
    const gaddr object = mode >= 0 && mode < 4 ? g_ld32(g, camera + 300u + 4u * static_cast<std::uint32_t>(mode)) : 0u;
    bool step = false;
    if (object != 0u) {
        const gaddr vtable = g_ld32(g, object);
        const std::int16_t adjust = g_ld16s(g, vtable + 0x68u);
        step = g_call1(g, g_ld32(g, vtable + 0x6Cu), object + static_cast<std::uint32_t>(static_cast<std::int32_t>(adjust))) == 1u;
    }
    if (step) g_call2(g, kFadeStep, manager, camera);
    else g_call1(g, kFadeReset, manager);
    std::memcpy(f.view2.data(), g_ptr(g, manager + kFadeValues, kFadeValuesSize), kFadeValuesSize);
    std::memcpy(g_ptr(g, manager + kFadeValues, kFadeValuesSize), view1.data(), kFadeValuesSize);
    static const bool trace = [] {
        const char *text = std::getenv("D012_TRACE_MAP_FADE");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    if (trace && s.frames % 15u == 0u) {
        std::string line;
        for (std::uint32_t i = 0; i < 8u; ++i) {
            if (g_ld32(g, manager + 0x6D0u + 4u * i) == 0u) continue;
            float v[2], w[2];
            std::memcpy(v, view1.data() + 12u * i, 8u);
            std::memcpy(w, f.view2.data() + 12u * i, 8u);
            char text[80];
            std::snprintf(text, sizeof text, " %u: %.2f->%.2f / %.2f->%.2f", i, v[0], v[1], w[0], w[1]);
            line += text;
        }
        std::printf("[fighter-fade] split %llu step %d, view 1 / view 2:%s\n", static_cast<unsigned long long>(s.frames),
                    step ? 1 : 0, line.c_str());
        std::fflush(stdout);
    }
}

// The split halves' framing (docs/port/local-versus.md, 7.14). Each half
// shows the middle 240 of its view's 480 columns, but the lock-on camera
// (mode 1, battle 0x08B40CDC) turns and pulls back to keep the fighters inside
// a frustum of the whole 480x272 screen: main 0x08882958(frustum, view, f12
// field of view in degrees, f13 aspect, f14 near, f15 far), with the aspect
// 480/272 written into the code. On a split frame those three frustums get
// the half's aspect, so the camera keeps the fighters inside the half.
// D012_SPLIT_FRAMING=0 leaves the game's aspect, as before.
//
// The halves' field of view (versus::split_fov_percent) scales the angle the
// camera update gives the projection (battle 0x08B3D25C, main 0x0887358C),
// view 2's in the split task, and the angle of those frustums.
constexpr std::uint32_t kFrustumBuild = 0x08882958u;
constexpr std::uint32_t kFramingCalls[] = {0x08B40EC8u, 0x08B40F10u, 0x08B40F4Cu};  // their return addresses
constexpr std::uint32_t kViewFovCall = 0x08B3D264u;  // the camera update's call of kProjectionSetFov returns here
constexpr float kHalfAspect = 240.0f / 272.0f;

bool framing_fix() {
    static const bool value = [] {
        const char *text = std::getenv("D012_SPLIT_FRAMING");
        return text == nullptr || std::strcmp(text, "0") != 0;
    }();
    return value;
}

float fov_scale() { return static_cast<float>(versus::split_fov_percent()) / 100.0f; }

// Whether the camera update about to run is for a split frame: the last frame
// was split, and no EX Burst or ending has made the next one full screen.
bool split_frame_expected(Guest *g) {
    const State &s = state();
    if (!versus::active() || !s.split_active || s.ex_burst != 0u) return false;
    const std::uint32_t phase = battle_phase(g);
    return phase == 0xFFFFFFFFu || phase < 8u;
}

void run_frustum_build(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    if (state().split_camera &&
        std::find(std::begin(kFramingCalls), std::end(kFramingCalls), in.gpr[31]) != std::end(kFramingCalls)) {
        regs.f[0] *= fov_scale();
        if (framing_fix()) regs.f[1] = kHalfAspect;
    }
    decomp::call_original(g, kFrustumBuild, regs);
    out = regs;
}

// D012_TRACE_FRAMING=1: how often each fighter's position (fighter +0x30) is
// outside the half that shows a view (columns 120 to 360 of its 480), from
// the view's view x projection (gfx +0x830), counted over split frames and
// logged every 300.
struct FramingCount {
    std::uint64_t frames = 0;
    std::uint64_t outside[2][2]{};  // [view][fighter: 0 the view's own player's, 1 the other]
    float least[2][2]{{480.0f, 480.0f}, {480.0f, 480.0f}};
    float most[2][2]{{0.0f, 0.0f}, {0.0f, 0.0f}};
};

bool trace_framing() {
    static const bool value = [] {
        const char *text = std::getenv("D012_TRACE_FRAMING");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    return value;
}

FramingCount &framing_count() {
    static FramingCount count;
    return count;
}

// A point's column in a 480-wide view, through the view x projection at
// `matrix` (four columns of four floats, as libGu keeps them); -1 behind the
// camera.
float view_column(Guest *g, gaddr matrix, gaddr point) {
    float m[16], p[3];
    std::memcpy(m, g_ptr(g, matrix, 64u), 64u);
    std::memcpy(p, g_ptr(g, point, 12u), 12u);
    const float x = p[0] * m[0] + p[1] * m[4] + p[2] * m[8] + m[12];
    const float w = p[0] * m[3] + p[1] * m[7] + p[2] * m[11] + m[15];
    return w > 0.0f ? 240.0f + 240.0f * x / w : -1.0f;
}

void count_framing(Guest *g, gaddr graphics, int view) {
    if (!trace_framing()) return;
    const gaddr field = g_ld32(g, kBattleManager);
    if (field == 0u) return;
    FramingCount &c = framing_count();
    for (int i = 0; i < 2; ++i) {
        // The view's own player's fighter first: player 1's for view 1.
        const gaddr fighter = g_ld32(g, field + ((i == 0) == (view == 0) ? 0x38u : 0x3Cu));
        if (fighter == 0u) continue;
        const float column = view_column(g, graphics + kViewProjection, fighter + 0x30u);
        if (column < 120.0f || column > 360.0f) ++c.outside[view][i];
        c.least[view][i] = std::min(c.least[view][i], column);
        c.most[view][i] = std::max(c.most[view][i], column);
    }
    if (view != 1) return;
    if (++c.frames % 300u != 0u) return;
    std::printf("[framing] %llu split frames, fov %d%%, framing fix %s: outside its half, view 1 player 1 %llu "
                "player 2 %llu, view 2 player 2 %llu player 1 %llu; columns view 1 %.0f..%.0f / %.0f..%.0f, "
                "view 2 %.0f..%.0f / %.0f..%.0f\n",
                static_cast<unsigned long long>(c.frames), versus::split_fov_percent(), framing_fix() ? "on" : "off",
                static_cast<unsigned long long>(c.outside[0][0]), static_cast<unsigned long long>(c.outside[0][1]),
                static_cast<unsigned long long>(c.outside[1][0]), static_cast<unsigned long long>(c.outside[1][1]),
                c.least[0][0], c.most[0][0], c.least[0][1], c.most[0][1], c.least[1][0], c.most[1][0], c.least[1][1],
                c.most[1][1]);
    std::fflush(stdout);
}

// main 0x0887358C (f12, projection): the projection's field of view, its
// first word.
void run_projection_set_fov(Guest *g, const decomp::CallState &in, GuestRegs &) {
    float fov = in.fpr[12];
    if (in.gpr[31] == kViewFovCall && state().split_camera) fov *= fov_scale();
    g_stf(g, in.gpr[4], fov);
}

void run_camera_update_thunk(Guest *g, const decomp::CallState &, GuestRegs &) {
    const gaddr manager = g_ld32(g, kCameraManager);
    state().split_camera = split_frame_expected(g);
    g_call1(g, kCameraUpdate, manager);
    state().split_camera = false;
    if (manager != 0u && versus::active()) step_fighter_fade_view2(g, manager);
}

// Player 2's values on the fighters, as the update sets player 1's.
void fighter_fade_to_view2(Guest *g, gaddr manager, FighterFadeSaved &saved) {
    const FighterFade &f = fighter_fade();
    if (f.owner != state().owner) return;
    for (std::uint32_t i = 0; i < 8u; ++i) {
        const gaddr slot_camera = g_ld32(g, manager + 0x6D0u + 4u * i);
        const gaddr fighter = slot_camera != 0u ? g_ld32(g, slot_camera + 0xF8u) : 0u;
        if (fighter == 0u) continue;
        saved.fighters.push_back({fighter, g_ld32(g, fighter + 0x380u), g_ld32(g, fighter + 0x490u)});
        float value;
        std::memcpy(&value, f.view2.data() + 12u * i, 4u);
        if (value < 1.0f) {
            g_st32(g, fighter + 0x380u, g_ld32(g, fighter + 0x380u) | 0x200000u);
            g_stf(g, fighter + 0x490u, value);
        } else {
            g_st32(g, fighter + 0x380u, g_ld32(g, fighter + 0x380u) & 0xFFDFFFFFu);
            g_stf(g, fighter + 0x490u, 1.0f);
        }
    }
}

void fighter_fade_back(Guest *g, const FighterFadeSaved &saved) {
    for (auto it = saved.fighters.rbegin(); it != saved.fighters.rend(); ++it) {
        g_st32(g, (*it)[0] + 0x380u, (*it)[1]);
        g_st32(g, (*it)[0] + 0x490u, (*it)[2]);
    }
}

// A model's draw (main 0x0886ACCC), as the game has it, but for the count at
// model +0x128: a model that lets go of its buffer (main 0x0886CA58) keeps it
// for two more draws, counted down here, then frees it (main 0x0886CAA0), so
// the list still being drawn can read it. Two draws a frame would free it a
// frame early; the second pass leaves the count alone. Calls from main
// 0x0886AED0, in the same unit, do not come through here.
constexpr std::uint32_t kModelDraw = 0x0886ACCCu;

void run_model_draw(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr model = in.gpr[4];
    const std::uint32_t a1 = in.gpr[5], a2 = in.gpr[6];
    g_call4(g, 0x0886A908u, model, a1, a2, in.gpr[7]);
    g_call1(g, 0x0886A54Cu, model);
    g_call1(g, 0x0886A180u, model);
    if (g_ld32(g, g_ld32(g, model + 0x84u)) != 0u) g_call1(g, 0x0886D618u, model);
    if (((g_ld32(g, model + 0x90u) & 1u) != 0u || a2 == 0u) && g_ld32(g, model + 0x128u) == 0u) {
        if (g_ld32(g, model + 0x94u) == 0u) g_call3(g, 0x0886E86Cu, model, a1, a2);
        if (g_ld32(g, model + 0x94u) != 0u) g_call2(g, 0x0886A24Cu, model, a1);
    } else {
        g_call2(g, 0x0886A3F4u, model, a1);
    }
    const std::uint32_t count = g_ld32(g, model + 0x128u);
    if (state().running) {
        static bool told = false;
        if (count != 0u && !told) {
            told = true;
            std::printf("[versus] a model's buffer count (%u) left alone in the second pass\n", count);
            std::fflush(stdout);
        }
        return;
    }
    if (count != 0u) {
        g_st32(g, model + 0x128u, count - 1u);
        if (count == 1u) g_call1(g, 0x0886CAA0u, model);
    }
}

// Player 2's lock-on cursor part: set up as the camera manager sets up its
// own (battle 0x08B41A54, then a sprite of the cursor's layout and the
// manager's part's two flags), once a battle, when the layout exists.
void set_up_cursor2(Guest *g, gaddr manager) {
    State &s = state();
    if (s.cursor2) return;
    const gaddr layout = g_ld32(g, kCursorLayout);
    if (layout == 0u) return;
    g_call1(g, kCursorInit, kPlayer2Cursor);
    g_call2(g, kCursorAttach, kPlayer2Cursor + 8u, layout);
    g_st8(g, kPlayer2Cursor, g_ld8(g, manager + kCursorPart));
    g_st8(g, kPlayer2Cursor + 1u, g_ld8(g, manager + kCursorPart + 1u));
    s.cursor2 = true;
    std::cout << "[versus] player 2's lock-on cursor set up (sprite " << std::hex << g_ld32(g, kPlayer2Cursor + 8u)
              << std::dec << ")" << std::endl;
}

// battle 0x08B41B24, the cursor part's release (the camera manager's
// teardown): player 2's part's sprite goes with player 1's.
void run_cursor_release(Guest *g, const decomp::CallState &in, GuestRegs &) {
    State &s = state();
    const gaddr parts[2] = {in.gpr[4], s.cursor2 ? kPlayer2Cursor : 0u};
    for (gaddr part : parts) {
        if (part == 0u) continue;
        const gaddr sprite = g_ld32(g, part + 8u);
        if (sprite != 0u) g_call1(g, kSpriteRelease, sprite);
        g_st32(g, part + 8u, 0u);
    }
    s.cursor2 = false;
}

// battle 0x08B41B40, the cursor shown or hidden (battle intros, EX Bursts):
// player 2's follows player 1's.
void run_cursor_enable(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr part = in.gpr[4];
    const std::uint32_t shown = in.gpr[5] & 0xFFu;
    const gaddr parts[2] = {part, state().cursor2 ? kPlayer2Cursor : 0u};
    for (gaddr p : parts) {
        if (p == 0u || g_ld8(g, p) != 1u) continue;
        g_st8(g, p + 1u, static_cast<std::uint8_t>(shown));
        g_call2(g, kCursorShow, p + 8u, shown);
    }
}

// D012_TRACE_WATCH=1, a tripwire for a crash in a versus battle: the music's
// refill (main 0x088A0264) reads from the disc through the file manager's
// reader, *(*(main 0x089D5EA0) + 4), and calls through the vtable pointer at
// its +0x154 (main 0x08862F74), which was found holding 0x3D000000. Each step
// of the split task checks that word; a change is logged with the step, and
// with what every camera slot's +0xF8 points at.
// D012_TRACE_HUD_LAYOUTS=1: the HUD's layouts (the menu manager *0x089D7830,
// its layouts linked from +0x24 through +0x24; one draws while its byte
// +0x14 has bit 1 and neither 0x0C nor 0x20), logged when one starts or stops
// drawing, with its first word and its sprites' count (linked through +0x3C).
void trace_hud_layouts(Guest *g) {
    static const bool on = [] {
        const char *text = std::getenv("D012_TRACE_HUD_LAYOUTS");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    if (!on) return;
    static std::vector<std::pair<gaddr, bool>> last;
    std::vector<std::pair<gaddr, bool>> now;
    const gaddr menus = g_ld32(g, 0x089D7830u);
    if (menus == 0u) return;
    for (gaddr layout = g_ld32(g, menus + 0x24u); layout != 0u && now.size() < 4096u;
         layout = g_ld32(g, layout + 0x24u)) {
        const std::uint8_t flags = g_ld8(g, layout + 0x14u);
        now.push_back({layout, (flags & 1u) != 0u && (flags & 0x2Cu) == 0u});
        // And each of its sprites (shown while u16 +0x20 has 0x8000), as
        // layout | 1 (sprite addresses are word aligned).
        std::uint32_t count = 0;
        for (gaddr sprite = g_ld32(g, layout + 0x3Cu); sprite != 0u && count < 512u;
             sprite = g_ld32(g, sprite + 0x3Cu), ++count)
            now.push_back({sprite | 1u, (g_ld16s(g, sprite + 0x20u) & 0x8000) != 0});
    }
    for (const auto &[layout, shown] : now) {
        bool was = false, known = false;
        for (const auto &[l, s] : last)
            if (l == layout) {
                was = s;
                known = true;
            }
        if (known && was == shown) continue;
        if ((layout & 1u) != 0u) {
            if (!known) continue;  // a sprite's first sighting: not logged
            const gaddr sprite = layout & ~1u;
            float xy[2];
            std::memcpy(xy, g_ptr(g, sprite + 0x10u, 8u), 8u);
            std::printf("[hud] split %llu sprite %08X %s (+0x24 %08X, x %.0f y %.0f)\n",
                        static_cast<unsigned long long>(state().frames), sprite, shown ? "shown" : "hidden",
                        g_ld32(g, sprite + 0x24u), xy[0], xy[1]);
            continue;
        }
        std::uint32_t sprites = 0;
        for (gaddr sprite = g_ld32(g, layout + 0x3Cu); sprite != 0u && sprites < 512u;
             sprite = g_ld32(g, sprite + 0x3Cu))
            ++sprites;
        std::printf("[hud] split %llu layout %08X %s (first word %08X, flags %02X, %u sprites)\n",
                    static_cast<unsigned long long>(state().frames), layout, shown ? "draws" : "stops",
                    g_ld32(g, layout), g_ld8(g, layout + 0x14u), sprites);
    }
    std::fflush(stdout);
    last = std::move(now);
}

bool watching() {
    static const bool on = [] {
        const char *text = std::getenv("D012_TRACE_WATCH");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    return on;
}

void watch(Guest *g, gaddr manager, const char *step) {
    if (!watching()) return;
    static gaddr last_reader = 0u;
    static std::uint32_t last_word = 0u;
    static std::uint32_t told = 0u;
    const gaddr files = g_ld32(g, 0x089D5EA0u);
    const gaddr reader = files != 0u ? g_ld32(g, files + 4u) : 0u;
    const std::uint32_t word = reader != 0u ? g_ld32(g, reader + 0x154u) : 0u;
    if ((reader != last_reader || word != last_word) && told < 200u) {
        ++told;
        std::string slots;
        for (std::uint32_t i = 0; i < 8u; ++i) {
            const gaddr camera = g_ld32(g, manager + 0x6D0u + 4u * i);
            char text[40];
            std::snprintf(text, sizeof text, " %u:%08X", i, camera != 0u ? g_ld32(g, camera + 0xF8u) : 0u);
            slots += text;
        }
        const gaddr field = g_ld32(g, kBattleManager);
        std::printf("[watch] split %llu %s: reader %08X +0x154 %08X -> reader %08X +0x154 %08X%s | fighters %08X "
                    "%08X | slot +0xF8%s\n",
                    static_cast<unsigned long long>(state().frames), step, last_reader, last_word, reader, word,
                    word != 0u && (word < 0x08800000u || word >= 0x0A000000u) ? " (NOT IN RAM)" : "",
                    field != 0u ? g_ld32(g, field + 0x38u) : 0u, field != 0u ? g_ld32(g, field + 0x3Cu) : 0u,
                    slots.c_str());
        std::fflush(stdout);
    }
    last_reader = reader;
    last_word = word;
}

void run_split_task(Guest *g, const decomp::CallState &in, GuestRegs &) {
    State &s = state();
    const std::uint32_t self = in.gpr[4];
    const std::uint32_t manager = g_ld32(g, kCameraManager);
    const std::uint32_t graphics = g_ld32(g, kGraphics);
    s.split_active = false;
    versus::note_split_frame(false);
    if (manager != 0u) watch(g, manager, "the frame's start (since the last split task)");
    if (versus::active()) trace_hud_layouts(g);
    if (versus::active()) trace_guide(g);
    if (!versus::active() || manager == 0u || graphics == 0u) return;
    // When the frame stays one full view, with player 2's cursor hidden:
    // - an EX Burst, from its start (run_ex_burst_start) until the
    //   battle-event bit it brings (either fighter's +0x5B1 bit 0x80, set at
    //   its start, battle 0x08A3EE44, and cleared at its end, 0x08A3F448) has
    //   come and gone, or ten seconds if the bit never comes;
    // - a fighter has fallen (the ending is the game's own, one view): its HP,
    //   the status block's (fighter +0x52C) maximum at +0x0C less the damage
    //   taken at +0x08 (battle 0x089E0110), is 0 or less.
    // A chase, the scripted (manager +0x804 == 2) and event (+0x9E0) cameras
    // and the other events that set the battle-event bit stay split.
    // D012_SPLIT_FULL_SCREEN_EVENTS=1 gives one full view for all of those
    // too, as before.
    static const bool full_screen_events = [] {
        const char *text = std::getenv("D012_SPLIT_FULL_SCREEN_EVENTS");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    const gaddr field = g_ld32(g, kBattleManager);
    const auto event_bit = [&](std::uint32_t slot_offset) {
        const gaddr fighter = field != 0u ? g_ld32(g, field + slot_offset) : 0u;
        return fighter != 0u && (g_ld8(g, fighter + 0x5B1u) & 0x80u) != 0u;
    };
    const auto fallen = [&](std::uint32_t slot_offset) {
        const gaddr fighter = field != 0u ? g_ld32(g, field + slot_offset) : 0u;
        const gaddr status = fighter != 0u ? g_ld32(g, fighter + 0x52Cu) : 0u;
        return status != 0u && g_ld32(g, status + 0x0Cu) != 0u &&
               g_ld32s(g, status + 0x0Cu) - g_ld32s(g, status + 0x08u) <= 0;
    };
    const bool bits = event_bit(0x38u) || event_bit(0x3Cu);
    if (s.ex_burst == 1u && bits) s.ex_burst = 2u;
    else if (s.ex_burst == 2u && !bits) s.ex_burst = 0u;
    else if (s.ex_burst == 1u && ++s.ex_burst_since > 300u) s.ex_burst = 0u;
    const std::uint32_t phase = battle_phase(g);
    const bool scripted = g_ld32(g, manager + 0x804u) == 2u;
    const bool event_camera = g_ld8(g, manager + 0x9E0u) == 1u;
    const bool camera_state = g_ld8(g, manager + 0x8BCu) == 1u;
    // What holds, logged when it changes: the camera manager's state
    // (+0x8BC, 1 in play), the scripted and event cameras, each fighter's
    // event bit and fall, the EX Burst, and the two fighters' camera modes
    // (+0x5AC, +0x108).
    {
        const auto mode = [&](std::uint32_t slot_offset) {
            const gaddr fighter = field != 0u ? g_ld32(g, field + slot_offset) : 0u;
            const gaddr camera = fighter != 0u ? g_ld32(g, fighter + 0x5ACu) : 0u;
            return camera != 0u ? g_ld32s(g, camera + 0x108u) : -9;
        };
        char reason[192];
        std::snprintf(reason, sizeof reason,
                      "phase %d manager state %u scripted %d event %d bits %d %d fallen %d %d EX Burst %u modes %d %d",
                      static_cast<std::int32_t>(phase), g_ld8(g, manager + 0x8BCu), scripted, event_camera, event_bit(0x38u), event_bit(0x3Cu),
                      fallen(0x38u), fallen(0x3Cu), s.ex_burst, mode(0x38u), mode(0x3Cu));
        static std::string last;
        static std::uint32_t told = 0;
        if (last != reason && told < 400u) {
            ++told;
            last = reason;
            std::printf("[versus] split %llu view state: %s\n", static_cast<unsigned long long>(s.frames), reason);
            std::fflush(stdout);
        }
    }
    // The camera manager's state is 0 for a moment at a Bravery break (a
    // battle event, battle 0x08ADF..., which flashes the screen): the frame
    // stays split, with view 2's camera built as in state 1 (below).
    if (!camera_state && !(g_ld8(g, manager + 0x8BCu) == 0u && phase <= 2u)) return;
    // The ending is one view from the white flash before the victory pose
    // (phase 8) on; the final blow and its replay (phases 1 to 3) stay split.
    const bool ending = phase != 0xFFFFFFFFu && phase >= 8u;
    const bool full = s.ex_burst != 0u || ending ||
                      (full_screen_events && (scripted || event_camera || bits || fallen(0x38u) || fallen(0x3Cu)));
    if (full) {
        if (s.cursor2) g_call2(g, kCursorShow, kPlayer2Cursor + 8u, 0u);
        return;
    }
    const int slot = opponent_slot(g, manager);
    if (slot < 0) return;
    // View 2 goes into a display list of its own (view2_list). Without one,
    // the frame's list must hold a second view about as long as the first:
    // when it would not, the frame stays one full view.
    const gaddr list2 = view2_list();
    {
        const gaddr context = g_ld32(g, kGuContext);
        const std::uint32_t start = g_ld32(g, context + 4u), cursor = g_ld32(g, context + 8u);
        const std::uint32_t size = g_ld32(g, context + 12u);
        const std::uint32_t used = cursor - start;
        s.peak1 = std::max(s.peak1, used);
        if (list2 == 0u && size != 0u && used < size && used > (size - 0x2000u) / 2u) {
            static std::uint32_t told = 0;
            if (told++ < 5u)
                std::printf("[versus] the display list is %u of %u bytes full: this frame is not split\n", used, size);
            if (s.cursor2) g_call2(g, kCursorShow, kPlayer2Cursor + 8u, 0u);
            return;
        }
    }
    // From here the frame is split (the HUD's placements read it).
    s.split_active = true;
    versus::note_split_frame(true);
    set_up_cursor2(g, manager);

    count_framing(g, graphics, 0);
    emit(g, kMarkerSplitLeft);
    ListSwitch list_switch;
    if (list2 != 0u) list_switch = list_to_view2(g, list2);

    std::uint8_t saved_view[kManagerView];
    std::uint8_t saved_camera[kGraphicsCameraSize];
    std::memcpy(saved_view, g_ptr(g, manager, kManagerView), kManagerView);
    std::memcpy(saved_camera, g_ptr(g, graphics + kGraphicsCamera, kGraphicsCameraSize), kGraphicsCameraSize);

    // The opponent's blended eye, look-at, up and field of view, as the
    // manager's update builds the on-screen camera's (battle 0x08B3CA24),
    // without the roll and the screen shake. The update moves the eye back
    // along the view by the slot camera's +0xD4 (in modes other than 2 and
    // 3); without that, a camera backed against a wall sits inside its
    // fighter, which then draws nothing.
    const std::uint32_t blended = manager + static_cast<std::uint32_t>(slot) * 0xB0u;
    float fov = 0.0f;
    std::memcpy(&fov, g_ptr(g, blended + 0x140u, 4u), 4u);
    {
        float eye[4], at[4];
        std::memcpy(eye, g_ptr(g, blended + 0x110u, 16u), 16u);
        std::memcpy(at, g_ptr(g, blended + 0x120u, 16u), 16u);
        const gaddr camera = g_ld32(g, manager + 0x6D0u + 4u * static_cast<std::uint32_t>(slot));
        const std::int32_t mode = camera != 0u ? g_ld32s(g, camera + 0x108u) : 2;
        if (mode != 2 && mode != 3) {
            float back;
            std::memcpy(&back, g_ptr(g, camera + 0xD4u, 4u), 4u);
            const float d[3] = {at[0] - eye[0], at[1] - eye[1], at[2] - eye[2]};
            const float length = std::sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
            if (length > 0.0f && std::isfinite(back))
                for (int k = 0; k < 3; ++k) eye[k] -= d[k] / length * back;
        }
        std::memcpy(g_ptr(g, kView2Eye, 16u), eye, 16u);
    }
    // D012_TRACE_VIEWS=1: every 15 frames, both views' eye and look-at and
    // both fighters' positions (fighter +0x30).
    static const bool trace_views = [] {
        const char *text = std::getenv("D012_TRACE_VIEWS");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    if (trace_views && s.frames % 15u == 0u) {
        const auto vec = [&](gaddr at) {
            float v[3];
            std::memcpy(v, g_ptr(g, at, 12u), 12u);
            char text[64];
            std::snprintf(text, sizeof text, "(%.2f %.2f %.2f)", v[0], v[1], v[2]);
            return std::string(text);
        };
        const gaddr field = g_ld32(g, kBattleManager);
        const gaddr f1 = g_ld32(g, field + 0x38u), f2 = g_ld32(g, field + 0x3Cu);
        const auto modes = [&](gaddr fighter) {
            const gaddr camera = fighter != 0u ? g_ld32(g, fighter + 0x5ACu) : 0u;
            char text[48];
            std::snprintf(text, sizeof text, "%d/%d/%d", camera != 0u ? g_ld32s(g, camera + 0x108u) : -9,
                          camera != 0u ? g_ld32s(g, camera + 0x10Cu) : -9, camera != 0u ? g_ld8(g, camera + 0x125u) : -9);
            return std::string(text);
        };
        std::printf("[views] split %llu view1 eye %s at %s | view2 eye %s at %s | p1 %s p2 %s | modes %s %s "
                    "mgr 804=%d 80c=%X 8b8=%d\n",
                    static_cast<unsigned long long>(s.frames), vec(manager + 0x40u).c_str(), vec(manager + 0x50u).c_str(),
                    vec(kView2Eye).c_str(), vec(blended + 0x120u).c_str(),
                    f1 != 0u ? vec(f1 + 0x30u).c_str() : "-", f2 != 0u ? vec(f2 + 0x30u).c_str() : "-",
                    modes(f1).c_str(), modes(f2).c_str(), g_ld32s(g, manager + 0x804u), g_ld32(g, manager + 0x80Cu),
                    g_ld32s(g, manager + 0x8B8u));
        std::fflush(stdout);
    }
    g_call2(g, kViewSetEye, manager, kView2Eye);
    g_call2(g, kViewSetAt, manager, blended + 0x120u);
    g_call2(g, kViewSetUp, manager, blended + 0x130u);
    g_call1(g, kViewBuild, manager);
    g_call_f4(g, kProjectionSetFov, fov * fov_scale(), manager + 0xC0u, 0u, 0u, 0u);
    g_call1(g, kProjectionBuild, manager + 0xC0u);
    const std::uint32_t effects = g_ld32(g, kEffectManager);
    if (effects != 0u) g_call3(g, kEffectCamera, effects, manager, manager + 0xC0u);

    // The graphics manager's camera from view 2, then the frame's starting
    // render state (with the screen cleared and the lights that follow the
    // camera), before anything is drawn.
    {
        // The graphics manager's camera from the manager's view is taken only
        // in state 1 (battle 0x08B3C0D8).
        const std::uint8_t camera_state_now = g_ld8(g, manager + 0x8BCu);
        g_st8(g, manager + 0x8BCu, 1u);
        g_call1(g, kCameraDraw, manager);
        g_st8(g, manager + 0x8BCu, camera_state_now);
    }
    frame_start_state(g, graphics);
    count_framing(g, graphics, 1);
    // Player 2's lock-on cursor, by the game's own cursor update, for player
    // 2's fighter while the camera manager holds view 2.
    if (s.cursor2) {
        const gaddr fighter = g_ld32(g, g_ld32(g, kBattleManager) + 0x3Cu);
        // The update shows the cursor only for the local player's fighter
        // (vtable +0x144, battle 0x08B2EF74: fighter +0x5B0 bit 4, which
        // battle 0x08B2EF44 sets on player 1's): player 2's counts as local
        // for this call only.
        if (fighter != 0u) {
            const std::uint8_t local = g_ld8(g, fighter + 0x5B0u);
            g_st8(g, fighter + 0x5B0u, static_cast<std::uint8_t>(local | 4u));
            g_call2(g, kCursorUpdate, kPlayer2Cursor, fighter);
            g_st8(g, fighter + 0x5B0u, static_cast<std::uint8_t>((g_ld8(g, fighter + 0x5B0u) & ~4u) | (local & 4u)));
        }
        static std::uint32_t lines = 0;
        if (trace_markers() && lines < 200u) {
            ++lines;
            const gaddr camera = fighter != 0u ? g_ld32(g, fighter + 0x5ACu) : 0u;
            const gaddr sprite = g_ld32(g, kPlayer2Cursor + 8u);
            std::printf("[markers] split %llu player 2's cursor part %u %u state %u camera mode %d/%d sprite flags %04X "
                        "player 1's part %u %u\n",
                        static_cast<unsigned long long>(s.frames), g_ld8(g, kPlayer2Cursor),
                        g_ld8(g, kPlayer2Cursor + 1u), g_ld32(g, kPlayer2Cursor + 0xCu),
                        camera != 0u ? g_ld32s(g, camera + 0x108u) : -9, camera != 0u ? g_ld32s(g, camera + 0x10Cu) : -9,
                        sprite != 0u ? static_cast<std::uint16_t>(g_ld16s(g, sprite + 0x20u)) : 0u,
                        g_ld8(g, manager + kCursorPart), g_ld8(g, manager + kCursorPart + 1u));
            std::fflush(stdout);
        }
    }
    watch(g, manager, "before the second pass");
    std::vector<std::pair<gaddr, std::array<std::uint32_t, 2>>> popups;
    popups_to_view2(g, popups);
    watch(g, manager, "popups to view 2");
    SlotTable view1_fade{};
    const bool faded = map_fade_to_view2(g, manager, slot, g_ld32(g, g_ld32(g, kBattleManager) + 0x3Cu), s.owner,
                                         view1_fade);
    watch(g, manager, "stage fade to view 2");
    FighterFadeSaved fighters_view1;
    fighter_fade_to_view2(g, manager, fighters_view1);
    watch(g, manager, "fighter fade to view 2");

    // The battle's draw tasks again, as the dispatcher (main 0x0888245C)
    // calls them.
    s.running = true;
    const std::uint32_t owner = g_ld32(g, self + 0x24u);
    const std::uint32_t mask = g_ld32(g, self + 0x44u);
    const std::uint32_t value = g_ld32(g, self + 0x48u);
    const std::uint32_t saved_running = g_ld32(g, kRunningTask);
    for (std::uint32_t t = g_ld32(g, owner + 0x4u); t != 0u;) {
        const std::uint32_t next = g_ld32(g, t + 0x34u);
        const std::int32_t priority = g_ld32s(g, t + 0x2Cu);
        const std::uint32_t fn = g_ld32(g, t);
        if (t != self && fn != 0u && g_ld32(g, t + 0x28u) == kDrawFilter && priority >= kFirstDrawPriority &&
            priority <= kLastDrawPriority && second_pass_runs(priority)) {
            if (s.frames == 0u)
                std::printf("[versus] second pass: task %08X priority %d function %08X\n", t, priority, fn);
            if (list2 != 0u && !view2_list_has_room(g, list_switch)) {
                // A view too long for its list: the rest of its draws are
                // left out this frame, rather than written past the end.
                static std::uint32_t told = 0;
                if (told++ < 5u)
                    std::printf("[versus] view 2's display list is full: draw tasks from priority %d left out\n",
                                priority);
                break;
            }
            g_st32(g, kRunningTask, t);
            g_st32(g, owner + 0x10u, t);
            g_st32(g, t + 0x44u, mask);
            g_st32(g, t + 0x48u, value);
            g_call1(g, fn, t);
            if (watching()) {
                char step[64];
                std::snprintf(step, sizeof step, "draw task %d (%08X)", priority, fn);
                watch(g, manager, step);
            }
        }
        t = next;
    }
    g_st32(g, kRunningTask, saved_running);
    g_st32(g, owner + 0x10u, self);
    s.running = false;
    if (list2 != 0u) {
        const std::uint32_t used2 = list_back(g, list_switch);
        if (used2 > s.peak2) {
            s.peak2 = used2;
            if (used2 > kList2Size / 2u)
                std::printf("[versus] view 2's display list: %u of %u bytes, the most so far\n", used2, kList2Size);
        }
    }
    if (trace_views && s.frames % 300u == 0u)
        std::printf("[views] split %llu display lists: view 1's list up to %u bytes at view 2's start, view 2's up "
                    "to %u\n",
                    static_cast<unsigned long long>(s.frames), s.peak1, s.peak2);
    for (const auto &[position, words] : popups) {
        g_st32(g, position, words[0]);
        g_st32(g, position + 4u, words[1]);
    }
    watch(g, manager, "popups back");
    if (faded) map_fade_back(g, view1_fade);
    watch(g, manager, "stage fade back");
    fighter_fade_back(g, fighters_view1);
    watch(g, manager, "fighter fade back");
    // Where both views leave the display list's write cursor (the context:
    // 4, the list's start, the cursor, its size, 0x58400 bytes).
    if (trace_views && s.frames % 15u == 0u) {
        const std::uint32_t context = g_ld32(g, kGuContext);
        std::printf("[views] split %llu list context 0x%08X: 0x%08X 0x%08X cursor 0x%08X 0x%08X\n",
                    static_cast<unsigned long long>(s.frames), context, g_ld32(g, context), g_ld32(g, context + 4u),
                    g_ld32(g, context + 8u), g_ld32(g, context + 12u));
        std::fflush(stdout);
    }

    std::memcpy(g_ptr(g, manager, kManagerView), saved_view, kManagerView);
    std::memcpy(g_ptr(g, graphics + kGraphicsCamera, kGraphicsCameraSize), saved_camera, kGraphicsCameraSize);
    if (effects != 0u) g_call3(g, kEffectCamera, effects, manager, manager + 0xC0u);
    g_call1(g, kScreenTarget, graphics);
    g_call1(g, kLoadCamera, graphics);
    camera_lights(g, graphics);
    // Both views side by side in the frame buffer, before the HUD
    // (MENU_MANAGER::Draw, after the battle's task list) draws over them.
    emit(g, kMarkerSplitCompose);
    if (s.frames++ == 0u)
        std::cout << "[versus] second view drawn (opponent camera slot " << slot << "); guest memory free "
                  << (portablekit::kernel().free_memory() >> 10u) << " KiB, largest block "
                  << (portablekit::kernel().largest_free_memory() >> 10u) << " KiB" << std::endl;
}

// Damage and Brave number popups (main 0x08858CB8, on *0x089D4DC8 + 0x190): at
// a hit the spawner projects a world point once through the on-screen view
// (the call returning to 0x08858D48 adds the object's offsets +0x33C/+0x340
// and a jitter of up to 15 pixels; the one returning to 0x08858D8C neither)
// and keeps the screen position in a ring of 10 (start +0x330, count +0x334,
// 0x50 bytes each, position at +0x20) that animates in screen space. They are
// drawn by a battle draw task (main 0x0888819C, priority 83000), so in both
// passes; the second pass gets each popup's place in view 2.
constexpr std::uint32_t kEffectsHud = 0x089D4DC8u;
constexpr std::uint32_t kPopups = 0x190u;
constexpr std::uint32_t kPopupWithOffsets = 0x08858D48u;
constexpr std::uint32_t kPopupAtPoint = 0x08858D8Cu;
constexpr std::uint32_t kPopupSlots = 10u;
constexpr float kOffscreenView = -1000.0f;  // a view x no half shows

struct PopupRecord {
    bool valid = false;
    float world[3]{};
    float base[2]{};  // where the projection put it, with the offsets: the entry minus this is its own motion
};

std::array<PopupRecord, kPopupSlots> &popup_records() {
    static std::array<PopupRecord, kPopupSlots> records;
    return records;
}

gaddr popup_ring(Guest *g) {
    const gaddr hud = g_ld32(g, kEffectsHud);
    return hud == 0u ? 0u : hud + kPopups;
}

// Called by the projection for the spawner's calls: the slot the spawner
// fills next is the end of the ring, or its start once the ring is full.
void note_popup(Guest *g, std::uint32_t caller, gaddr point, gaddr result) {
    const gaddr ring = popup_ring(g);
    if (ring == 0u) return;
    const std::int32_t start = g_ld32s(g, ring + 0x330u);
    const std::int32_t count = g_ld32s(g, ring + 0x334u);
    const std::int32_t slot = count < 10 ? (start + count) % 10 : start;
    if (slot < 0 || slot >= 10) return;
    PopupRecord &r = popup_records()[static_cast<std::size_t>(slot)];
    std::memcpy(r.world, g_ptr(g, point, 12u), 12u);
    std::memcpy(r.base, g_ptr(g, result, 8u), 8u);
    if (caller == kPopupWithOffsets) {
        r.base[0] += static_cast<float>(g_ld32s(g, ring + 0x33Cu));
        r.base[1] += static_cast<float>(g_ld32s(g, ring + 0x340u));
    }
    r.valid = true;
}

// Before the second pass: each live popup to its place in view 2 (the
// graphics manager's view x projection is view 2's by then), off screen when
// it is behind view 2's camera; `saved` keeps the positions to put back.
void popups_to_view2(Guest *g, std::vector<std::pair<gaddr, std::array<std::uint32_t, 2>>> &saved) {
    const gaddr ring = popup_ring(g);
    const gaddr graphics = g_ld32(g, kGraphics);
    if (ring == 0u || graphics == 0u) return;
    const std::int32_t start = g_ld32s(g, ring + 0x330u);
    const std::int32_t count = g_ld32s(g, ring + 0x334u);
    if (count == 0) {
        // The ring is empty (or was reset for a battle): no record belongs to
        // a live popup any more.
        for (PopupRecord &r : popup_records()) r.valid = false;
        return;
    }
    if (start < 0 || start >= 10 || count < 0 || count > 10) return;
    for (std::int32_t i = 0; i < count; ++i) {
        const std::uint32_t slot = static_cast<std::uint32_t>((start + i) % 10);
        // The spawner fills exactly the slot note_popup worked out, right
        // after the projection that recorded it, so a live slot's record is
        // its own popup's.
        const PopupRecord &r = popup_records()[slot];
        const gaddr position = ring + slot * 0x50u + 0x20u;
        float now[2];
        std::memcpy(now, g_ptr(g, position, 8u), 8u);
        if (!r.valid) continue;
        saved.push_back({position, {g_ld32(g, position), g_ld32(g, position + 4u)}});
        const gaddr frame = g_push(g, 0x20u);
        for (int k = 0; k < 3; ++k) g_stf(g, frame + 4u * static_cast<std::uint32_t>(k), r.world[k]);
        g_stf(g, frame + 0xCu, 1.0f);
        g_call3(g, kMatrixApplyProjected, frame + 0x10u, graphics + kViewProjection, frame);
        float clip[4];
        std::memcpy(clip, g_ptr(g, frame + 0x10u, 16u), 16u);
        g_pop(g, 0x20u);
        if (clip[3] < 0.0f) {
            g_stf(g, position, kOffscreenView);
            continue;
        }
        const float x = (clip[0] * 0.5f + 0.5f) * 480.0f;
        const float y = 272.0f - (clip[1] * 0.5f + 0.5f) * 272.0f;
        g_stf(g, position, x + (now[0] - r.base[0]));
        g_stf(g, position + 4u, y + (now[1] - r.base[1]));
    }
}

// main 0x08866E04, world to screen: (out, in) -> 1 when the point is in front
// of the camera. Copies `in` with w = 1, takes it through the graphics
// manager's view x projection (gfx+0x830, by main 0x089A4AF8)
// and, when w is not negative, scales x and y onto the 480x272 screen. The
// HUD places its world-pinned pieces with it.
// D012_TRACE_MARKERS=1 logs the calls, a few frames' worth.
void run_world_to_screen(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    const gaddr result = in.gpr[4];
    const gaddr point = in.gpr[5];
    const gaddr frame = g_push(g, 0x30u);
    g_call2(g, kVec4Copy, frame, point);
    g_stf(g, frame + 0xCu, 1.0f);
    g_call3(g, kMatrixApplyProjected, frame + 0x10u, g_ld32(g, kGraphics) + kViewProjection, frame);
    float clip[4];
    std::memcpy(clip, g_ptr(g, frame + 0x10u, 16u), 16u);
    g_pop(g, 0x30u);
    out.v0 = clip[3] < 0.0f ? 0u : 1u;
    if (out.v0 != 0u) {
        g_stf(g, result + 0xCu, 0.0f);
        g_stf(g, result + 0x8u, 0.0f);
        g_stf(g, result + 0x0u, (clip[0] * 0.5f + 0.5f) * 480.0f);
        g_stf(g, result + 0x4u, 272.0f - (clip[1] * 0.5f + 0.5f) * 272.0f);
        if (in.gpr[31] == kPopupWithOffsets || in.gpr[31] == kPopupAtPoint) note_popup(g, in.gpr[31], point, result);
    }
    static const bool trace = [] {
        const char *text = std::getenv("D012_TRACE_MARKERS");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    static std::uint32_t lines = 0;
    if (trace && lines < 3000u) {
        ++lines;
        float p[3];
        std::memcpy(p, g_ptr(g, point, 12u), 12u);
        float s[2] = {0.0f, 0.0f};
        if (out.v0 != 0u) std::memcpy(s, g_ptr(g, result, 8u), 8u);
        std::printf("[markers] split %llu caller %08X world (%.2f, %.2f, %.2f) -> %s (%.1f, %.1f)\n",
                    static_cast<unsigned long long>(state().frames), in.gpr[31], p[0], p[1], p[2],
                    out.v0 != 0u ? "screen" : "behind", s[0], s[1]);
        std::fflush(stdout);
    }
}

bool trace_markers() {
    static const bool value = [] {
        const char *text = std::getenv("D012_TRACE_MARKERS");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    return value;
}

// Each half of the window shows the middle half of its view, guest x 120 to
// 360: a view's x in the window's x, or kOffscreen outside that.
constexpr float kOffscreen = -1000.0f;
float window_x(float view_x, int view) {
    if (view_x < 120.0f || view_x >= 360.0f) return kOffscreen;
    return view_x - 120.0f + 240.0f * static_cast<float>(view);
}

// main 0x088D9C60, a layout instance's position: (instance, x, y) as
// integers, stored as floats at +0x10/+0x14, and flag 0x800 of the u16 at
// +0x20 cleared. On a split frame, the lock-on cursor (placed from player
// 1's view by battle 0x08B418C8) goes to its place in the left half.
// D012_TRACE_MARKERS=1 logs who places HUD sprites where.
void run_set_position(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr instance = in.gpr[4];
    float x = static_cast<float>(static_cast<std::int32_t>(in.gpr[5]));
    const float y = static_cast<float>(static_cast<std::int32_t>(in.gpr[6]));
    const bool cursor = in.gpr[31] == kCursorCaller && state().split_active;
    const bool cursor2 = cursor && state().cursor2 && instance == g_ld32(g, kPlayer2Cursor + 8u);
    if (cursor) x = window_x(x, cursor2 ? 1 : 0);
    g_st16(g, instance + 0x20u, static_cast<std::uint16_t>(g_ld16s(g, instance + 0x20u) & 0xF7FF));
    g_stf(g, instance + 0x10u, x);
    g_stf(g, instance + 0x14u, y);
    static std::uint32_t lines = 0;
    if (trace_markers() && lines < 6000u) {
        ++lines;
        std::printf("[markers] split %llu place %08X caller %08X at (%d, %d)%s\n",
                    static_cast<unsigned long long>(state().frames), instance, in.gpr[31],
                    static_cast<std::int32_t>(in.gpr[5]), static_cast<std::int32_t>(in.gpr[6]),
                    cursor2 ? " (player 2's cursor, right half)" : cursor ? " (player 1's cursor, left half)" : "");
        std::fflush(stdout);
    }
}

struct Install {
    Install() {
        if (!on()) return;
        static const decomp::Override overrides[] = {
            {kSetPosition, "versus_set_position", decomp::Module::Main, decomp::Returns::Nothing, &run_set_position},
            {kCursorRelease, "versus_cursor_release", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
             &run_cursor_release},
            {kCursorEnable, "versus_cursor_enable", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
             &run_cursor_enable},
            {kWorldToScreen, "versus_world_to_screen", decomp::Module::Main, decomp::Returns::Integer,
             &run_world_to_screen},
            {kModelDraw, "versus_model_draw", decomp::Module::Main, decomp::Returns::Nothing, &run_model_draw},
            {kCameraUpdateThunk, "versus_camera_update_thunk", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
             &run_camera_update_thunk},
            {kCameraDrawThunk, "versus_camera_draw_thunk", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
             &run_camera_draw_thunk},
            {kSplitTask, "versus_split_view_task", decomp::Module::Main, decomp::Returns::Nothing, &run_split_task},
            {kExBurstStart, "versus_ex_burst_start", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
             &run_ex_burst_start},
            {kWinnerDecision, "versus_winner_decision", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
             &run_winner_decision},
            {kSlotWon, "versus_slot_won", decomp::Module::Main, decomp::Returns::Integer, &run_slot_won},
            {kGuideDriver, "versus_guide_driver", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
             &run_guide_driver},
            {kGuideMode, "versus_guide_mode", decomp::Module::BattleOverlay, decomp::Returns::Integer,
             &run_guide_mode},
            {kSpriteColour, "versus_sprite_colour", decomp::Module::Main, decomp::Returns::Nothing,
             &run_sprite_colour},
            {kFrustumBuild, "versus_frustum_build", decomp::Module::Main, decomp::Returns::Nothing,
             &run_frustum_build},
            {kProjectionSetFov, "versus_projection_set_fov", decomp::Module::Main, decomp::Returns::Nothing,
             &run_projection_set_fov},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
        // Overrides that only trace, there only with their switch.
        const char *trace = std::getenv("D012_TRACE_GUIDE");
        if (trace != nullptr && *trace != '\0' && std::strcmp(trace, "0") != 0) {
            static const decomp::Override traces[] = {
                {kGuideRequest, "versus_guide_request", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
                 &run_guide_request},
            };
            decomp::add_overrides(traces, sizeof traces / sizeof traces[0], true);
        }
        std::cout << "[versus] split view built in: a versus battle is drawn a second time from player 2's camera"
                  << std::endl;
    }
} install;

} // namespace
} // namespace d012::split_view
