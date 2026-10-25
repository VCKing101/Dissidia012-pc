// Local versus, the second view (docs/port/local-versus.md, section 2): a
// spike, on only with D012_LOCAL_VERSUS=1.
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
// Nothing else of section 2.3 is handled yet: the spike is there to see what
// breaks.
// While a scripted or event camera is in charge (intros, EX Bursts) there is
// no second view.
//
// The task is added from an override of the camera manager's draw-task thunk
// (battle 0x08B3C43C), which runs every battle frame inside the battle's task
// dispatch: the running task (main 0x089D69B0) names the battle's task owner.
//
// MIT licence; see port/README.md.

#include "d012_decomp_harness.hpp"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

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
// battle
constexpr std::uint32_t kCameraManager = 0x08B3D6B8u;    // -> camera manager
constexpr std::uint32_t kCameraDrawThunk = 0x08B3C43Cu;  // OBJ_CAMERA_MANAGER::DrawTask
constexpr std::uint32_t kCameraDraw = 0x08B3C0D8u;       // (manager)
// No code is ever loaded below the EBOOT; the task's function and name live
// here.
constexpr std::uint32_t kSplitTask = 0x08800200u;
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

bool on() {
    static const bool value = [] {
        const char *text = std::getenv("D012_LOCAL_VERSUS");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    return value;
}

struct State {
    std::uint32_t owner = 0;  // the battle task owner the split task was added to
    std::uint32_t task = 0;
    bool running = false;     // inside the second pass
    std::uint64_t frames = 0;
};

State &state() {
    static State s;
    return s;
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
    std::cout << "[versus] split-view task added to the battle's task list" << std::endl;
}

void run_camera_draw_thunk(Guest *g, const decomp::CallState &, GuestRegs &) {
    g_call1(g, kCameraDraw, g_ld32(g, kCameraManager));
    if (!state().running) add_task(g);
}

// The opponent's camera slot in the manager, or -1.
int opponent_slot(Guest *g, std::uint32_t manager) {
    const std::uint32_t field = g_ld32(g, kBattleManager);
    if (field == 0u) return -1;
    const std::uint32_t fighter = g_ld32(g, field + 0x3Cu);
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

void run_split_task(Guest *g, const decomp::CallState &in, GuestRegs &) {
    State &s = state();
    const std::uint32_t self = in.gpr[4];
    const std::uint32_t manager = g_ld32(g, kCameraManager);
    const std::uint32_t graphics = g_ld32(g, kGraphics);
    if (manager == 0u || graphics == 0u || g_ld8(g, manager + 0x8BCu) != 1u) return;
    // A scripted camera (manager +0x804 == 2) or an event camera (+0x9E0):
    // the frame stays one full view.
    if (g_ld32(g, manager + 0x804u) == 2u || g_ld8(g, manager + 0x9E0u) == 1u) return;
    const int slot = opponent_slot(g, manager);
    if (slot < 0) return;

    emit(g, kMarkerSplitLeft);

    std::uint8_t saved_view[kManagerView];
    std::uint8_t saved_camera[kGraphicsCameraSize];
    std::memcpy(saved_view, g_ptr(g, manager, kManagerView), kManagerView);
    std::memcpy(saved_camera, g_ptr(g, graphics + kGraphicsCamera, kGraphicsCameraSize), kGraphicsCameraSize);

    // The opponent's blended eye, look-at, up and field of view, as the
    // manager's update builds the on-screen camera's (battle 0x08B3CA24),
    // without the roll and the screen shake.
    const std::uint32_t blended = manager + static_cast<std::uint32_t>(slot) * 0xB0u;
    float fov = 0.0f;
    std::memcpy(&fov, g_ptr(g, blended + 0x140u, 4u), 4u);
    g_call2(g, kViewSetEye, manager, blended + 0x110u);
    g_call2(g, kViewSetAt, manager, blended + 0x120u);
    g_call2(g, kViewSetUp, manager, blended + 0x130u);
    g_call1(g, kViewBuild, manager);
    g_call_f4(g, kProjectionSetFov, fov, manager + 0xC0u, 0u, 0u, 0u);
    g_call1(g, kProjectionBuild, manager + 0xC0u);
    const std::uint32_t effects = g_ld32(g, kEffectManager);
    if (effects != 0u) g_call3(g, kEffectCamera, effects, manager, manager + 0xC0u);

    g_call1(g, kScreenTarget, graphics);
    g_call1(g, kGuClear, 0x17u);

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
            priority <= kLastDrawPriority) {
            g_st32(g, kRunningTask, t);
            g_st32(g, owner + 0x10u, t);
            g_st32(g, t + 0x44u, mask);
            g_st32(g, t + 0x48u, value);
            g_call1(g, fn, t);
        }
        t = next;
    }
    g_st32(g, kRunningTask, saved_running);
    g_st32(g, owner + 0x10u, self);
    s.running = false;

    std::memcpy(g_ptr(g, manager, kManagerView), saved_view, kManagerView);
    std::memcpy(g_ptr(g, graphics + kGraphicsCamera, kGraphicsCameraSize), saved_camera, kGraphicsCameraSize);
    if (effects != 0u) g_call3(g, kEffectCamera, effects, manager, manager + 0xC0u);
    g_call1(g, kScreenTarget, graphics);
    g_call1(g, kLoadCamera, graphics);
    // Both views side by side in the frame buffer, before the HUD
    // (MENU_MANAGER::Draw, after the battle's task list) draws over them.
    emit(g, kMarkerSplitCompose);
    if (s.frames++ == 0u) std::cout << "[versus] second view drawn (opponent camera slot " << slot << ")" << std::endl;
}

struct Install {
    Install() {
        if (!on()) return;
        static const decomp::Override overrides[] = {
            {kCameraDrawThunk, "versus_camera_draw_thunk", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
             &run_camera_draw_thunk},
            {kSplitTask, "versus_split_view_task", decomp::Module::Main, decomp::Returns::Nothing, &run_split_task},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
        std::cout << "[versus] local versus spike on: the battle is drawn a second time from the opponent's camera"
                  << std::endl;
    }
} install;

} // namespace
} // namespace d012::split_view
