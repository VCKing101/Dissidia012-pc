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
#include "d012_versus.hpp"

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

void run_camera_update_thunk(Guest *g, const decomp::CallState &, GuestRegs &) {
    const gaddr manager = g_ld32(g, kCameraManager);
    g_call1(g, kCameraUpdate, manager);
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

void run_split_task(Guest *g, const decomp::CallState &in, GuestRegs &) {
    State &s = state();
    const std::uint32_t self = in.gpr[4];
    const std::uint32_t manager = g_ld32(g, kCameraManager);
    const std::uint32_t graphics = g_ld32(g, kGraphics);
    s.split_active = false;
    if (!versus::active() || manager == 0u || graphics == 0u || g_ld8(g, manager + 0x8BCu) != 1u) return;
    // A scripted camera (manager +0x804 == 2), an event camera (+0x9E0), or a
    // battle event between the fighters, such as an EX Burst (either
    // fighter's +0x5B1 bit 0x80, set at its start, battle 0x08A3EE44, and
    // cleared at its end, 0x08A3F448): the frame stays one full view, and
    // player 2's cursor is hidden.
    // The same once a fighter has fallen (the ending is the game's own, one
    // view): its HP, the status block's (fighter +0x52C) maximum at +0x0C less
    // the damage taken at +0x08 (battle 0x089E0110), is 0 or less.
    const gaddr field = g_ld32(g, kBattleManager);
    const auto in_event = [&](std::uint32_t slot_offset) {
        const gaddr fighter = field != 0u ? g_ld32(g, field + slot_offset) : 0u;
        if (fighter == 0u) return false;
        if ((g_ld8(g, fighter + 0x5B1u) & 0x80u) != 0u) return true;
        const gaddr status = g_ld32(g, fighter + 0x52Cu);
        return status != 0u && g_ld32(g, status + 0x0Cu) != 0u &&
               g_ld32s(g, status + 0x0Cu) - g_ld32s(g, status + 0x08u) <= 0;
    };
    if (g_ld32(g, manager + 0x804u) == 2u || g_ld8(g, manager + 0x9E0u) == 1u || in_event(0x38u) ||
        in_event(0x3Cu)) {
        if (s.cursor2) g_call2(g, kCursorShow, kPlayer2Cursor + 8u, 0u);
        return;
    }
    const int slot = opponent_slot(g, manager);
    if (slot < 0) return;
    // The display list (libGu's context: +4 its start, +8 the write cursor,
    // +0xC its size, 0x58400 bytes) must hold a second view about as long as
    // the first: when it would not, the frame stays one full view. Both views
    // reach 86% of it on Planet's Core (7.5).
    {
        const gaddr context = g_ld32(g, kGuContext);
        const std::uint32_t start = g_ld32(g, context + 4u), cursor = g_ld32(g, context + 8u);
        const std::uint32_t size = g_ld32(g, context + 12u);
        const std::uint32_t used = cursor - start;
        if (size != 0u && used < size && used > (size - 0x2000u) / 2u) {
            static std::uint32_t told = 0;
            if (told++ < 5u)
                std::printf("[versus] the display list is %u of %u bytes full: this frame is not split\n", used, size);
            if (s.cursor2) g_call2(g, kCursorShow, kPlayer2Cursor + 8u, 0u);
            return;
        }
    }
    // From here the frame is split (the HUD's placements read it).
    s.split_active = true;
    set_up_cursor2(g, manager);

    emit(g, kMarkerSplitLeft);

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
    g_call_f4(g, kProjectionSetFov, fov, manager + 0xC0u, 0u, 0u, 0u);
    g_call1(g, kProjectionBuild, manager + 0xC0u);
    const std::uint32_t effects = g_ld32(g, kEffectManager);
    if (effects != 0u) g_call3(g, kEffectCamera, effects, manager, manager + 0xC0u);

    // The graphics manager's camera from view 2, then the frame's starting
    // render state (with the screen cleared and the lights that follow the
    // camera), before anything is drawn.
    g_call1(g, kCameraDraw, manager);
    frame_start_state(g, graphics);
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
    std::vector<std::pair<gaddr, std::array<std::uint32_t, 2>>> popups;
    popups_to_view2(g, popups);
    SlotTable view1_fade{};
    const bool faded = map_fade_to_view2(g, manager, slot, g_ld32(g, g_ld32(g, kBattleManager) + 0x3Cu), s.owner,
                                         view1_fade);
    FighterFadeSaved fighters_view1;
    fighter_fade_to_view2(g, manager, fighters_view1);

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
    if (faded) map_fade_back(g, view1_fade);
    fighter_fade_back(g, fighters_view1);
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
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
        std::cout << "[versus] split view built in: a versus battle is drawn a second time from player 2's camera"
                  << std::endl;
    }
} install;

} // namespace
} // namespace d012::split_view
