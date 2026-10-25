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

#include <array>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
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
constexpr std::uint32_t kSystemTasks = 0x089D69A8u;      // -> the system task list (its owner at +0x2000)
constexpr std::uint32_t kMenuManager = 0x089D7830u;      // -> the menu manager (the HUD's layouts)
constexpr std::uint32_t kMenuDraw = 0x088CED50u;         // (menu manager): MENU_MANAGER::Draw's body
constexpr std::uint32_t kHudPriority = 70035u;           // MENU_MANAGER::Draw's, in the system list
constexpr std::uint32_t kLockPoint = 0x0883820Cu;        // (fighter, out vec4): where a lock-on aims
// battle: the lock-on cursor, the camera manager's part at +0x878 (battle
// 0x08B41BDC, run by the camera update for the on-screen camera's fighter);
// its layout instance at +0x880, placed by the call at battle 0x08B418C8.
constexpr std::uint32_t kCursorPart = 0x878u;
constexpr std::uint32_t kCursorCaller = 0x08B418C8u;
// Below the EBOOT, as the split task: player 2's cursor task, its name, and
// view 2's view x projection for it.
constexpr std::uint32_t kCursorTask = 0x08800280u;
constexpr std::uint32_t kCursorTaskName = 0x088002C0u;
constexpr std::uint32_t kView2Matrix = 0x08800300u;
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
    // The frame was drawn split: the second view was drawn this frame (set at
    // the end of the split task), and still so when the next frame's update
    // places the HUD, until a frame that is not split.
    bool split_active = false;
    std::uint32_t cursor_task = 0;  // in the system task list, after the HUD's draw
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

void add_cursor_task(Guest *g);
void popups_to_view2(Guest *g, std::vector<std::pair<gaddr, std::array<std::uint32_t, 2>>> &saved);

void run_split_task(Guest *g, const decomp::CallState &in, GuestRegs &) {
    State &s = state();
    const std::uint32_t self = in.gpr[4];
    const std::uint32_t manager = g_ld32(g, kCameraManager);
    const std::uint32_t graphics = g_ld32(g, kGraphics);
    s.split_active = false;
    if (manager == 0u || graphics == 0u || g_ld8(g, manager + 0x8BCu) != 1u) return;
    // A scripted camera (manager +0x804 == 2) or an event camera (+0x9E0):
    // the frame stays one full view.
    if (g_ld32(g, manager + 0x804u) == 2u || g_ld8(g, manager + 0x9E0u) == 1u) return;
    const int slot = opponent_slot(g, manager);
    if (slot < 0) return;
    add_cursor_task(g);

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

    // The graphics manager's camera from view 2, then the frame's starting
    // render state (with the screen cleared and the lights that follow the
    // camera), before anything is drawn.
    g_call1(g, kCameraDraw, manager);
    frame_start_state(g, graphics);
    // View 2's view x projection, for player 2's lock-on cursor after the HUD.
    std::memcpy(g_ptr(g, kView2Matrix, 64u), g_ptr(g, graphics + kViewProjection, 64u), 64u);
    std::vector<std::pair<gaddr, std::array<std::uint32_t, 2>>> popups;
    popups_to_view2(g, popups);

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
    for (const auto &[position, words] : popups) {
        g_st32(g, position, words[0]);
        g_st32(g, position + 4u, words[1]);
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
    s.split_active = true;
    if (s.frames++ == 0u) std::cout << "[versus] second view drawn (opponent camera slot " << slot << ")" << std::endl;
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
    if (start < 0 || start >= 10 || count <= 0 || count > 10) return;
    for (std::int32_t i = 0; i < count; ++i) {
        const std::uint32_t slot = static_cast<std::uint32_t>((start + i) % 10);
        const PopupRecord &r = popup_records()[slot];
        const gaddr position = ring + slot * 0x50u + 0x20u;
        float now[2];
        std::memcpy(now, g_ptr(g, position, 8u), 8u);
        // A record from this popup's own projection: its own motion stays
        // within the jitter and the rise.
        if (!r.valid || std::fabs(now[0] - r.base[0]) > 64.0f || std::fabs(now[1] - r.base[1]) > 160.0f) continue;
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
    if (cursor) x = window_x(x, 0);
    g_st16(g, instance + 0x20u, static_cast<std::uint16_t>(g_ld16s(g, instance + 0x20u) & 0xF7FF));
    g_stf(g, instance + 0x10u, x);
    g_stf(g, instance + 0x14u, y);
    static std::uint32_t lines = 0;
    if (trace_markers() && lines < 6000u) {
        ++lines;
        std::printf("[markers] split %llu place %08X caller %08X at (%d, %d)%s\n",
                    static_cast<unsigned long long>(state().frames), instance, in.gpr[31],
                    static_cast<std::int32_t>(in.gpr[5]), static_cast<std::int32_t>(in.gpr[6]),
                    cursor ? " (player 1's cursor, left half)" : "");
        std::fflush(stdout);
    }
}

// Player 2's lock-on cursor. After the HUD's draw (system list, priority
// 70035, after MENU_MANAGER::Draw), the menu manager's layouts are drawn
// again with everything but the cursor hidden, the cursor moved to where
// player 2's lock-on target lies in view 2 (view 2's view x projection, kept
// by the split task), in the right half. The cursor's look (its animation,
// its scale) stays player 1's: it is one sprite.
void add_cursor_task(Guest *g) {
    State &s = state();
    if (s.cursor_task != 0u) return;
    // The system task list's owner, as MENU_MANAGER registers its tasks with
    // it (main 0x08882274 of *0x089D69A8: the word at +0x2000).
    const std::uint32_t list = g_ld32(g, kSystemTasks);
    if (list == 0u) return;
    const std::uint32_t owner = g_ld32(g, list + 0x2000u);
    if (owner == 0u) return;
    const char name[] = "D012::Player2CursorTask";
    std::memcpy(g_ptr(g, kCursorTaskName, sizeof name), name, sizeof name);
    s.cursor_task = g_call5(g, kTaskFactory, owner, kDrawFilter, kHudPriority, kCursorTask, kCursorTaskName);
    std::cout << "[versus] player 2's lock-on cursor task added to the system task list" << std::endl;
}

// Player 2's lock-on target, as battle 0x08B41BDC finds player 1's: the
// camera's lock-on mode (camera +0x108 == 1), its mode object's vtable slot
// +0x70 with the camera.
gaddr lock_target(Guest *g, gaddr fighter) {
    const gaddr camera = g_ld32(g, fighter + 0x5ACu);
    if (camera == 0u || g_ld32s(g, camera + 0x108u) != 1) return 0u;
    const gaddr mode = g_ld32(g, camera + 300u + 4u);
    if (mode == 0u) return 0u;
    const gaddr vtable = g_ld32(g, mode);
    const std::int16_t adjust = g_ld16s(g, vtable + 0x70u);
    const gaddr fn = g_ld32(g, vtable + 0x74u);
    return g_call2(g, fn, mode + static_cast<std::uint32_t>(static_cast<std::int32_t>(adjust)), camera);
}

void run_cursor_task(Guest *g, const decomp::CallState &, GuestRegs &) {
    State &s = state();
    if (!s.split_active) return;
    const gaddr menu = g_ld32(g, kMenuManager);
    const gaddr manager = g_ld32(g, kCameraManager);
    const gaddr field = g_ld32(g, kBattleManager);
    if (menu == 0u || manager == 0u || field == 0u) return;
    const gaddr cursor = g_ld32(g, manager + kCursorPart + 8u);
    const gaddr fighter = g_ld32(g, field + 0x3Cu);
    if (cursor == 0u || fighter == 0u) return;
    if ((static_cast<std::uint16_t>(g_ld16s(g, cursor + 0x20u)) & 0x8000u) == 0u) return;  // not drawn now
    const gaddr target = lock_target(g, fighter);
    if (target == 0u) return;
    const gaddr frame = g_push(g, 0x20u);
    g_call2(g, kLockPoint, target, frame);
    g_stf(g, frame + 0xCu, 1.0f);
    g_call3(g, kMatrixApplyProjected, frame + 0x10u, kView2Matrix, frame);
    float clip[4];
    std::memcpy(clip, g_ptr(g, frame + 0x10u, 16u), 16u);
    g_pop(g, 0x20u);
    if (clip[3] < 0.0f) return;
    const float x = window_x((clip[0] * 0.5f + 0.5f) * 480.0f, 1);
    const float y = 272.0f - (clip[1] * 0.5f + 0.5f) * 272.0f;
    if (x == kOffscreen) return;

    // The cursor's layout; every other layout and sprite hidden for this draw.
    struct Saved {
        gaddr address;
        std::uint32_t value;
        int size;
    };
    std::vector<Saved> saved;
    gaddr cursor_layout = 0u;
    std::uint32_t guard = 0u;
    for (gaddr layout = g_ld32(g, menu + 0x24u); layout != 0u && guard < 4096u; layout = g_ld32(g, layout + 0x24u)) {
        for (gaddr sprite = g_ld32(g, layout); sprite != 0u && ++guard < 4096u; sprite = g_ld32(g, sprite + 0x3Cu))
            if (sprite == cursor) cursor_layout = layout;
    }
    if (cursor_layout == 0u) return;
    guard = 0u;
    for (gaddr layout = g_ld32(g, menu + 0x24u); layout != 0u && ++guard < 4096u; layout = g_ld32(g, layout + 0x24u)) {
        saved.push_back({layout + 0x14u, g_ld8(g, layout + 0x14u), 1});
        if (layout != cursor_layout) g_st8(g, layout + 0x14u, static_cast<std::uint8_t>(g_ld8(g, layout + 0x14u) | 0x20u));
    }
    for (gaddr sprite = g_ld32(g, cursor_layout); sprite != 0u && ++guard < 8192u; sprite = g_ld32(g, sprite + 0x3Cu)) {
        if (sprite == cursor) continue;
        saved.push_back({sprite + 0x20u, static_cast<std::uint16_t>(g_ld16s(g, sprite + 0x20u)), 2});
        saved.push_back({sprite + 0x30u, g_ld32(g, sprite + 0x30u), 4});
        g_st16(g, sprite + 0x20u, static_cast<std::uint16_t>(g_ld16s(g, sprite + 0x20u) & 0x7FFF));
        g_st32(g, sprite + 0x30u, 0u);
    }
    saved.push_back({cursor + 0x10u, g_ld32(g, cursor + 0x10u), 4});
    saved.push_back({cursor + 0x14u, g_ld32(g, cursor + 0x14u), 4});
    g_stf(g, cursor + 0x10u, x);
    g_stf(g, cursor + 0x14u, y);
    g_call1(g, kMenuDraw, menu);
    for (auto it = saved.rbegin(); it != saved.rend(); ++it) {
        if (it->size == 1) g_st8(g, it->address, static_cast<std::uint8_t>(it->value));
        else if (it->size == 2) g_st16(g, it->address, static_cast<std::uint16_t>(it->value));
        else g_st32(g, it->address, it->value);
    }
    static std::uint32_t lines = 0;
    if (trace_markers() && lines < 3000u) {
        ++lines;
        std::printf("[markers] split %llu player 2's cursor at (%.0f, %.0f)\n",
                    static_cast<unsigned long long>(s.frames), x, y);
        std::fflush(stdout);
    }
}

struct Install {
    Install() {
        if (!on()) return;
        static const decomp::Override overrides[] = {
            {kSetPosition, "versus_set_position", decomp::Module::Main, decomp::Returns::Nothing, &run_set_position},
            {kCursorTask, "versus_player2_cursor_task", decomp::Module::Main, decomp::Returns::Nothing,
             &run_cursor_task},
            {kWorldToScreen, "versus_world_to_screen", decomp::Module::Main, decomp::Returns::Integer,
             &run_world_to_screen},
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
