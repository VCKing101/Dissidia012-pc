// The right-stick camera in battle (docs/port/right-stick-camera.md).
//
// The battle camera's two input modes, free and lock-on, each have a
// per-frame update the camera manager calls through the mode's vtable. Both
// run as decompiled overrides (port/decomp/battle_camera.c) with a hook
// between the stock input, which still reads the D-pad, recentre and
// auto-follow, and the code that builds the view. The hook adds the right
// stick as the owner's PPSSPP plugin did: full deflection is one stock D-pad
// step, a partial push is proportional, with the game's speed table (the
// camera-speed option), its distance limits and its two direction options.
// Only the local player's camera listens, and only while the game itself
// would take camera input.
//
// The stick is read from the pad the game reads (sceCtrlReadBufferPositive's
// buffer, bytes 10 and 11, which the game never uses); R3, a button a PSP
// lacks, presses SELECT, the game's own recentre, while a battle camera takes
// input.
//
// The free camera's auto-follow, which turns the Story Mode world map camera
// toward where the walker faces, gets an angular dead zone while the port's
// analog stick steers the walker (docs/port/analog-movement.md, "Camera
// follow"): the free input (battle 0x08B3F7F4) runs as decompiled C, reached
// from the decompiled free update.
//
// MIT licence; see port/README.md.

#include "d012_camera.hpp"
#include "snapshot.hpp"
#include "d012_comm_battle.hpp"
#include "d012_decomp_harness.hpp"
#include "d012_decomp_pad.hpp"
#include "d012_input.hpp"
#include "d012_net.hpp"
#include "d012_versus.hpp"

#include "../decomp/battle_camera.h"

#include "camera/camera_input.hpp"
#include "hle/hle_common.hpp"
#include "portablekit/hle_extension.hpp"

#include "psprecomp/runtime.hpp"

#include <any>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

namespace d012::camera {
namespace {

namespace ext = portablekit::hle_extension;

constexpr std::uint32_t kSceCtrlReadBufferPositive = 0x1F803938u;
constexpr std::uint32_t kSelect = 0x0001u;
constexpr std::uint32_t kHold = 0x00020000u;
// A battle camera must have taken input within this many pad reads for R3
// to press SELECT (one read a frame).
constexpr std::uint32_t kLiveReads = 4u;

bool flag(const char *name) {
    const char *value = std::getenv(name);
    return value != nullptr && *value != '\0' && std::strcmp(value, "0") != 0;
}

float number(const char *name, float fallback, float low, float high) {
    const char *value = std::getenv(name);
    if (value == nullptr || *value == '\0') return fallback;
    char *end = nullptr;
    const float parsed = std::strtof(value, &end);
    if (end == value || !std::isfinite(parsed)) return fallback;
    return std::clamp(parsed, low, high);
}

struct Options {
    bool enabled = true;
    // The mouse pushes player 1's right stick (add_mouse below).
    bool mouse = true;
    // The plugin's: each axis's dead zone, the response rescaled to start
    // from 0 at its edge. On top of PortableKit's radial dead zone for the
    // stick (the menu's Dead zone), as the plugin's was on top of PPSSPP's.
    float dead_zone = 0.18f;
    // Full deflection as a share of the stock D-pad speed. Lock-on caps the
    // visible turn at the D-pad speed whatever this is.
    float sensitivity = 1.0f;
    bool invert_x = false;
    bool invert_y = false;
    bool trace = false;
    // The differential test checks the hooked updates themselves rather than
    // the plain decompiled ones: SAME while the stick is centred.
    bool verify_hook = false;
    // The follow's angular dead zone, degrees each side of straight ahead;
    // 0: the game's follow.
    float follow_dead_zone = 22.5f;
    bool trace_follow = false;
};

const Options &options() {
    static const Options o = [] {
        Options v;
        v.enabled = !flag("D012_NO_RIGHT_STICK_CAMERA");
        v.mouse = v.enabled && !flag("D012_NO_MOUSE_CAMERA");
        v.dead_zone = number("D012_CAMERA_DEAD_ZONE", v.dead_zone, 0.0f, 0.9f);
        v.sensitivity = number("D012_CAMERA_SENSITIVITY", v.sensitivity, 0.05f, 4.0f);
        v.invert_x = flag("D012_CAMERA_INVERT_X");
        v.invert_y = flag("D012_CAMERA_INVERT_Y");
        v.trace = flag("D012_TRACE_STICK_CAMERA");
        v.verify_hook = flag("D012_CAMERA_VERIFY_HOOK");
        v.follow_dead_zone = number("D012_CAMERA_FOLLOW_DEADZONE", v.follow_dead_zone, 0.0f, 60.0f);
        v.trace_follow = flag("D012_TRACE_CAMERA_FOLLOW");
        return v;
    }();
    return o;
}

// What the last pad read held.
struct Pad {
    std::uint8_t x = 0x80u;
    std::uint8_t y = 0x80u;
    std::uint32_t reads = 0;
    // The read at which the local camera last took input; far in the past
    // at first.
    std::uint32_t live_read = 0xFFFF0000u;
    bool r3 = false;
};

Pad &pad() {
    static Pad p;
    return p;
}

// -1..1 with the plugin's per-axis dead zone. PortableKit centres the stick
// on 0x80 and pushes it 127 each way.

float stick_axis(std::uint8_t raw, float zone) {
    const float value = std::clamp((static_cast<float>(raw) - 128.0f) / 127.0f, -1.0f, 1.0f);
    float magnitude = std::fabs(value);
    if (magnitude <= zone) return 0.0f;
    magnitude = std::min((magnitude - zone) / (1.0f - zone), 1.0f);
    return value < 0.0f ? -magnitude : magnitude;
}

// The byte stick_axis reads back as `value` with dead zone `zone`.
std::uint8_t stick_byte(float value, float zone) {
    if (value == 0.0f) return 0x80u;
    const float raw = zone + std::min(std::fabs(value), 1.0f) * (1.0f - zone);
    const int offset = std::min(static_cast<int>(std::lround(raw * 127.0f)), 127);
    return static_cast<std::uint8_t>(value < 0.0f ? 128 - offset : 128 + offset);
}

// The mouse. PortableKit turns its motion into degrees (the menu's Mouse
// sensitivity and its two inversions) for a game's camera driver to take, and
// drops what nothing took at the next flip. This port registers no driver
// (its menu rows are for another game's camera), so the motion is taken here,
// in the camera update between the flip that brought it and the next.
// - The free camera (and the Story Mode world map's) turns by it directly:
//   its degrees go straight onto the yaw, on top of the stick, with no speed
//   cap, and vertical motion moves the camera closer or further by the
//   distance the stick's full push gives for the degrees it turns.
// - Lock-on, which caps its turn at the stock speed anyway, gets it as a push
//   of player 1's right stick (add_mouse). A full push is the turn the local
//   camera last made for one (its stock step with the camera-speed option,
//   times the sensitivity); what one update cannot turn is carried to the
//   next.
// In a linked battle the sending copy picks by its camera's latest update
// (send_mouse), and the degrees ride in the pad the link sends.
struct Mouse {
    float yaw = 0.0f;  // degrees carried to the next update
    float pitch = 0.0f;
    float full = 9.0f;  // degrees a full push turns in one update
    std::chrono::steady_clock::time_point last{};
    // The local camera's latest update: when, and whether it was free.
    std::chrono::steady_clock::time_point updated{};
    bool free = false;
    // What the link's 1/100 and 1/4 degree steps left over, for the next frame.
    float sent_yaw = 0.0f;
    float sent_pitch = 0.0f;
};

// A linked battle's direct mouse degrees for each player, from the pads the
// link exchanged, until that player's camera takes them.
struct Linked {
    float yaw = 0.0f;
    float pitch = 0.0f;
};
Linked linked[2];

// The mouse's degrees since the last take, for the direct turn; none with
// the mouse camera off.
struct Turn {
    float yaw = 0.0f;
    float pitch = 0.0f;
};
Turn take_mouse() {
    if (!options().mouse) return {};
    const portablekit::camera::Turn taken = portablekit::camera::take(portablekit::camera::Source::Mouse);
    return {taken.yaw_degrees, taken.pitch_degrees};
}

Mouse &mouse() {
    static Mouse m;
    return m;
}

struct CameraSaved {
    Pad pad;
    Mouse mouse;
    Linked linked[2];
};

void add_state_to_snapshots() {
    portablekit::snapshot::add(
        "camera", [] { return std::any(CameraSaved{pad(), mouse(), {linked[0], linked[1]}}); },
        [](const std::any &any) {
            const CameraSaved &saved = std::any_cast<const CameraSaved &>(any);
            pad() = saved.pad;
            mouse() = saved.mouse;
            linked[0] = saved.linked[0];
            linked[1] = saved.linked[1];
        });
}
const bool added_to_snapshots = (add_state_to_snapshots(), true);

// The local player's camera with manual input on, while the game takes
// input: the gates of the game's own pad accessors (input muted, a replay
// playing) and hardware feeding pad 0 (not ad-hoc).
// In local versus (d012_versus.cpp) the camera on logical pad 1 is player
// 2's, fed by player 2's pad: 0 for player 1's camera, 1 for player 2's, -1
// for any other.
int local_camera(Guest *g, gaddr camera) {
    if (g_ld8(g, camera + CAM_MANUAL) == 0) return -1;
    const gaddr pads = g_ld32(g, G_PAD_MANAGER);
    if (pads == 0) return -1;
    if ((g_ld32(g, pads + PAD_FLAGS) & (PAD_FLAG_REPLAY | PAD_FLAG_MUTED)) != 0) return -1;
    const gaddr listens = g_ld32(g, camera + CAM_PAD);
    // In a Communication Mode battle on the port's link, both fighters'
    // cameras listen: each to its own player's injected pad.
    if (comm_battle::active(g)) {
        if (listens == pads + PAD_LOGICAL0) return 0;
        if (listens == pads + PAD_LOGICAL0 + PAD_LOGICAL_SIZE) return 1;
        return -1;
    }
    if (listens == pads + PAD_LOGICAL0) return g_ld32s(g, pads + PAD_RAW0_MODE) != 1 ? 0 : -1;
    if (versus::active() && listens == pads + PAD_LOGICAL0 + PAD_LOGICAL_SIZE) return 1;
    return -1;
}

// What the stick asks of this camera: turn > 0 is what D-pad right does,
// closer > 0 what D-pad up does, after the player's two direction options.
// The player's two camera direction options, as the game's own camera keys
// follow them: from the camera's logical pad's table (its +0x4C, entries of
// u64 raw source and u64 logical camera key), which main 0x0887309C rewrites
// from the options, rather than from the options' copy at G_PLAYER_FLAGS.
// That copy is only filled as a battle is set up: on the first Story Mode
// world map after loading a save it is still 0, and the stick there turned
// against the player's horizontal option until the first battle.
// flip_x: D-pad left (or the nub's left) turns as the yaw's + key, so the
// stick's right must turn the yaw -; flip_y: D-pad down is the closer key.
struct Directions {
    bool flip_x = false;
    bool flip_y = false;
};

constexpr gaddr kPadKeyTable = 0x4Cu;  // logical pad: -> its camera key table
constexpr std::uint32_t kDpadDown = 0x40u, kDpadLeft = 0x80u;  // raw source, low word
constexpr std::uint32_t kNubDown = 0x10u, kNubLeft = 0x20u;    // raw source, high word

Directions directions(Guest *g, gaddr camera, std::uint16_t flags) {
    Directions d{(flags & PLAYER_INVERT_HORIZONTAL) != 0, (flags & PLAYER_INVERT_VERTICAL) != 0};
    const gaddr logical = g_ld32(g, camera + CAM_PAD);
    const gaddr table = logical != 0u ? g_ld32(g, logical + kPadKeyTable) : 0u;
    if (table == 0u) return d;
    const auto source = [&](gaddr key, bool &flip, std::uint32_t low, std::uint32_t high) {
        const std::uint32_t key_lo = g_ld32(g, key), key_hi = g_ld32(g, key + 4u);
        // The template has 30 entries; the bound only guards a bad pointer.
        for (gaddr e = table; e < table + 64u * 16u; e += 16u) {
            const std::uint32_t lo = g_ld32(g, e), hi = g_ld32(g, e + 4u);
            if (lo == 0u && hi == 0u) return;
            if (g_ld32(g, e + 8u) != key_lo || g_ld32(g, e + 12u) != key_hi) continue;
            flip = (lo == low && hi == 0u) || (lo == 0u && hi == high);
            return;
        }
    };
    source(G_FREE_MASK_TURN_POS, d.flip_x, kDpadLeft, kNubLeft);
    source(G_FREE_MASK_CLOSER, d.flip_y, kDpadDown, kNubDown);
    return d;
}

// The mouse's direct turn for one camera update: yaw > 0 turns as D-pad right
// does and closer > 0 moves as D-pad up does, in degrees, after the same
// direction options as the stick.
struct Look {
    float yaw = 0.0f;
    float closer = 0.0f;
};

// `full` is the degrees a full push turns this camera in this update, before
// the sensitivity; the local player's sizes the mouse's push. `free`: the
// free camera's update, where the mouse turns it directly (`look`); in
// lock-on it pushes the stick.
bool camera_stick(Guest *g, gaddr camera, float full, bool free, float &turn, float &closer, Look &look) {
    const int player = local_camera(g, camera);
    if (player < 0) return false;
    const auto player_flags = [&]() {
        return static_cast<std::uint16_t>(g_ld32(g, G_PLAYER_FLAGS + 4u * static_cast<std::uint32_t>(player)) &
                                          0xFFFFu);
    };
    const auto note_update = [&]() {
        Mouse &m = mouse();
        m.updated = std::chrono::steady_clock::now();
        if (free && !m.free) m.yaw = m.pitch = 0.0f;  // lock-on's carry is not for the free camera
        m.free = free;
    };
    if (comm_battle::active(g)) {
        // That player's right stick as injected this frame (raw pad i's sample,
        // bytes 10 and 11), with that player's own camera settings (each copy
        // sends its player's as the battle links) and the game's own options
        // for that player: the same turn on both copies.
        const net::battle::Camera settings = net::battle::camera(player);
        // This copy's mouse is already in its player's sample (the battle link
        // sent it); the next push it gives is sized by this camera.
        if (player == comm_battle::local_player(g)) {
            mouse().full = full * settings.sensitivity;
            note_update();
        }
        const gaddr sample = g_ld32(g, G_PAD_MANAGER) + static_cast<std::uint32_t>(player) * 0x60u + 0x40u;
        turn = stick_axis(g_ld8(g, sample + 10u), settings.dead_zone);
        closer = -stick_axis(g_ld8(g, sample + 11u), settings.dead_zone);
        // The direct degrees the link brought for this player, on both copies.
        Linked &l = linked[player];
        look.yaw = l.yaw;
        look.closer = -l.pitch;
        l = Linked{};
        if (turn == 0.0f && closer == 0.0f && look.yaw == 0.0f && look.closer == 0.0f) return false;
        const Directions game = directions(g, camera, player_flags());
        const bool flip_x = settings.invert_x != game.flip_x;
        const bool flip_y = settings.invert_y != game.flip_y;
        turn *= flip_x ? -settings.sensitivity : settings.sensitivity;
        closer *= flip_y ? -settings.sensitivity : settings.sensitivity;
        if (flip_x) look.yaw = -look.yaw;
        if (flip_y) look.closer = -look.closer;
        return true;
    }
    Pad &p = pad();
    // R3 presses SELECT only for a battle combatant's camera (time step
    // source 0, battle 0x08B0E200): the Story Mode world map runs this
    // camera too (source 1, main 0x08918E90), and SELECT is not its recentre.
    const bool battle = g_ld32s(g, camera + CAM_STEP_SOURCE) == 0;
    std::uint8_t x = p.x, y = p.y;
    if (player == 1) {
        if (battle) versus::note_camera_live();
        const versus::RightStick stick = versus::player2_right_stick();
        x = stick.x;
        y = stick.y;
    } else {
        if (battle) p.live_read = p.reads;
    }
    // The mouse turns the camera of the player who has the keyboard: player
    // 2's when Controls > Player 2 is the keyboard and mouse.
    if ((player == 1) == portablekit::hle_extension::second_pad().keyboard) {
        mouse().full = full * options().sensitivity;
        note_update();
        if (free) {
            const Turn taken = take_mouse();
            look.yaw = taken.yaw;
            look.closer = -taken.pitch;  // as add_mouse adds it to the stick's Y
        } else {
            add_mouse(x, y);
        }
    }
    turn = stick_axis(x, options().dead_zone);
    closer = -stick_axis(y, options().dead_zone);  // the Y axis grows downward
    if (turn == 0.0f && closer == 0.0f && look.yaw == 0.0f && look.closer == 0.0f) return false;
    const Options &o = options();
    // Each player's own direction options (slot 1's are copied from player 1's).
    const Directions game = directions(g, camera, player_flags());
    const bool flip_x = o.invert_x != game.flip_x;
    const bool flip_y = o.invert_y != game.flip_y;
    turn *= flip_x ? -o.sensitivity : o.sensitivity;
    closer *= flip_y ? -o.sensitivity : o.sensitivity;
    if (flip_x) look.yaw = -look.yaw;
    if (flip_y) look.closer = -look.closer;
    return true;
}

// battle 0x08B3B4FC: the camera's time step, in 1/60 s (2.0 at 30 Hz).
float time_step(Guest *g, gaddr camera) {
    float step = 0.0f;
    switch (g_ld32s(g, camera + CAM_STEP_SOURCE)) {
    case 0: std::memcpy(&step, g_ptr(g, g_ld32(g, G_FRAME_STEP_PTR) + 0x18u, 4), 4); break;
    case 1: std::memcpy(&step, g_ptr(g, G_FRAME_STEP, 4), 4); break;
    case 2: std::memcpy(&step, g_ptr(g, camera + CAM_STEP_OWN, 4), 4); break;
    default: break;
    }
    return step;
}

float ldf(Guest *g, gaddr address) {
    float value;
    std::memcpy(&value, g_ptr(g, address, 4), 4);
    return value;
}

gaddr row_of(Guest *g, gaddr camera) {
    return g_ld32(g, G_CAMERA_ROWS) + static_cast<std::uint32_t>(g_ld32s(g, camera + CAM_ROW)) * ROW_SIZE;
}

std::uint32_t speed_index(Guest *g, gaddr camera) {
    return static_cast<std::uint32_t>(std::clamp(g_ld32s(g, camera + CAM_SPEED_INDEX), 0, 2));
}

constexpr float kPi = 3.14159265f;
constexpr float kDegrees = kPi / 180.0f;

void trace(const char *mode, float turn, float closer, float yaw, float distance) {
    static std::uint32_t lines = 0;
    if (!options().trace || lines >= 4000u) return;
    ++lines;
    std::printf("[camera] read %u %s turn %+.3f closer %+.3f -> yaw %+.4f distance %.3f\n", pad().reads, mode, turn,
                closer, yaw, distance);
    std::fflush(stdout);
}

void trace_look(const char *mode, const Look &look) {
    static std::uint32_t lines = 0;
    if (!options().trace || lines >= 4000u || (look.yaw == 0.0f && look.closer == 0.0f)) return;
    ++lines;
    std::printf("[camera] read %u %s mouse direct %+.2f degrees, closer %+.2f\n", pad().reads, mode, look.yaw,
                look.closer);
    std::fflush(stdout);
}

void announce(const char *mode) {
    static bool free_seen = false, lock_seen = false;
    bool &seen = std::strcmp(mode, "free") == 0 ? free_seen : lock_seen;
    if (seen) return;
    seen = true;
    std::cout << "[camera] the right stick turned the " << mode << " camera" << std::endl;
}

// After the free camera's stock input (battle 0x08B3F7F4).
void free_hook(Guest *g, gaddr mode, gaddr camera) {
    const gaddr row = row_of(g, camera);
    const float step = time_step(g, camera);
    const float speed = ldf(g, row + ROW_FREE_YAW_SPEED + 4u * speed_index(g, camera));
    float turn = 0.0f, closer = 0.0f;
    Look look;
    if (!camera_stick(g, camera, speed * step, true, turn, closer, look)) return;
    // The mouse's degrees on top of the stick's step; its vertical degrees move
    // the camera as far as the stick moves it while turning that many.
    const float distance_speed = ldf(g, row + ROW_FREE_DISTANCE_SPEED);
    const float per_degree = speed > 0.0f ? distance_speed / speed : 0.0f;
    float yaw = ldf(g, mode + FREE_YAW) + turn * kDegrees * speed * step + look.yaw * kDegrees;
    // The stick turns well under half a circle a frame, so one wrap is
    // enough for it; a fast mouse can turn more.
    while (yaw > kPi) yaw -= 2.0f * kPi;
    while (yaw < -kPi) yaw += 2.0f * kPi;
    g_stf(g, mode + FREE_YAW, yaw);
    const float distance = std::clamp(ldf(g, mode + FREE_DISTANCE) - closer * distance_speed * step - look.closer * per_degree,
                                      ldf(g, row + ROW_FREE_MIN), ldf(g, row + ROW_FREE_MAX));
    g_stf(g, mode + FREE_DISTANCE, distance);
    announce("free");
    trace("free", turn, closer, yaw, distance);
    trace_look("free", look);
}

// After the lock-on camera's stock input (battle 0x08B40290), which has just
// written this frame's turn from the D-pad; the game applies it, at most at
// the stock speed, later in the same update.
void lock_hook(Guest *g, gaddr mode, gaddr camera) {
    const gaddr row = row_of(g, camera);
    const float step = time_step(g, camera);
    const float speed = ldf(g, row + ROW_LOCK_YAW_SPEED + 4u * speed_index(g, camera));
    float turn = 0.0f, closer = 0.0f;
    // The mouse pushes the stick here; direct degrees reach lock-on only in a
    // linked battle, sent while this camera was still free.
    Look look;
    if (!camera_stick(g, camera, speed * step, false, turn, closer, look)) return;
    const float distance_speed = ldf(g, row + ROW_LOCK_DISTANCE_SPEED);
    const float per_degree = speed > 0.0f ? distance_speed / speed : 0.0f;
    const float delta = ldf(g, mode + LOCK_YAW_DELTA) + turn * kDegrees * speed * step + look.yaw * kDegrees;
    g_stf(g, mode + LOCK_YAW_DELTA, delta);
    const float distance = std::clamp(ldf(g, mode + LOCK_DISTANCE) - closer * distance_speed * step - look.closer * per_degree,
                                      ldf(g, row + ROW_LOCK_MIN), ldf(g, row + ROW_LOCK_MAX));
    g_stf(g, mode + LOCK_DISTANCE, distance);
    announce("lock-on");
    trace("lock-on", turn, closer, delta, distance);
    trace_look("lock-on", look);
}

namespace decomp = d012::decomp;

// ---- The auto-follow's dead zone -----------------------------------------------

// On with the smooth stick (D012_NO_SMOOTH_STICK=1 is the game's) and a zone.
bool follow_zone_on() {
    static const bool on = decomp::smooth_stick_on() && options().follow_dead_zone > 0.0f;
    return on;
}

void trace_follow(Guest *g, gaddr mode, gaddr camera, const CameraFollowTrace &t, bool zoned, bool analog,
                  const PadAnalog &stick) {
    static std::uint32_t calls = 0, lines = 0;
    ++calls;
    if (lines >= 40000u) return;
    ++lines;
    const gaddr owner = g_ld32(g, camera + CAM_OWNER);
    const float yaw = ldf(g, mode + FREE_YAW);
    // The stick's angle from straight ahead (up), right positive.
    const float stick_angle = std::atan2(stick.x, -stick.y) / kDegrees;
    if (t.followed)
        std::printf("[follow] %u cam %08X step %u yaw %+9.4f deg in %+9.4f target %+9.4f diff %+9.4f fed %+9.4f "
                    "turn %+8.4f %s stick %s %+7.2f %.2f pos (%.3f, %.3f)\n",
                    calls, camera, static_cast<unsigned>(g_ld32s(g, camera + CAM_STEP_SOURCE)), yaw / kDegrees,
                    t.yaw_in / kDegrees, t.target / kDegrees, t.diff / kDegrees, t.fed / kDegrees,
                    std::remainder(t.yaw_out - t.yaw_in, 2.0f * kPi) / kDegrees, zoned ? "zone" : "game", analog ? "port" : "none",
                    stick_angle, stick.magnitude, ldf(g, owner + 0x30u), ldf(g, owner + 0x38u));
    else
        std::printf("[follow] %u cam %08X step %u yaw %+9.4f deg (no follow) stick %s %+7.2f %.2f pos (%.3f, %.3f)\n",
                    calls, camera, static_cast<unsigned>(g_ld32s(g, camera + CAM_STEP_SOURCE)), yaw / kDegrees,
                    analog ? "port" : "none", stick_angle, stick.magnitude, ldf(g, owner + 0x30u),
                    ldf(g, owner + 0x38u));
}

// battle 0x08B3F7F4, the free input: the zone applies while the camera's
// owner is walked by the port's stick (so not in battle, and not with the
// D-pad control option). Reached from the decompiled free update below.
void check_free_input(Guest *g, const decomp::CallState &in, GuestRegs &);

// Whether the follow's gates are open for this call (as battle 0x08B3FAA4
// tests them): the differential test checks those calls as a probe of
// their own, the first calls being at rest.
bool following(Guest *g, gaddr camera) {
    if (g_ld8(g, camera + CAM_MANUAL) == 0 || g_ld8(g, camera + CAM_FOLLOW) != 1u) return false;
    const gaddr owner = g_ld32(g, camera + CAM_OWNER);
    return g_ld8(g, owner + 0x542u) <= 1u && (g_ld8(g, owner + 0x5B0u) & 0x08u) != 0;
}

void run_free_input(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const gaddr mode = in.gpr[4], camera = in.gpr[5];
    if (decomp::verifying() && following(g, camera)) {
        GuestRegs args{};
        args.a[0] = mode;
        args.a[1] = camera;
        decomp::verify_probe("camera_free_input (following)", F_FREE_INPUT, decomp::Returns::Nothing,
                             &check_free_input, in, args);
    }
    PadAnalog stick{};
    const bool analog = decomp::analog_walker(g_ld32(g, camera + CAM_OWNER), &stick);
    const Options &o = options();
    const bool zoned = follow_zone_on() && analog;
    // The ramp, half the zone, ends (33.75 degrees by default) short of
    // where the game's follow settles for a diagonal push (about 35 degrees,
    // the walker's facing lagging its walk while the camera turns), so a
    // diagonal still turns at the game's full rate.
    const CameraFollowZone zone{o.follow_dead_zone * kDegrees, o.follow_dead_zone * 0.5f * kDegrees};
    CameraFollowTrace t{};
    camera_free_input(g, mode, camera, zoned ? &zone : nullptr, &t);
    if (o.trace_follow) trace_follow(g, mode, camera, t, zoned, analog, stick);
}
void check_free_input(Guest *g, const decomp::CallState &in, GuestRegs &) {
    camera_free_input(g, in.gpr[4], in.gpr[5], nullptr, nullptr);
}

void run_free(Guest *g, const decomp::CallState &in, GuestRegs &) {
    camera_free_update(g, in.gpr[4], in.gpr[5], &free_hook);
}
void check_free(Guest *g, const decomp::CallState &in, GuestRegs &) {
    camera_free_update(g, in.gpr[4], in.gpr[5], nullptr);
}
void run_lock(Guest *g, const decomp::CallState &in, GuestRegs &) {
    camera_lock_update(g, in.gpr[4], in.gpr[5], &lock_hook);
}
void check_lock(Guest *g, const decomp::CallState &in, GuestRegs &) {
    camera_lock_update(g, in.gpr[4], in.gpr[5], nullptr);
}

// The two updates are reached only through their modes' vtables (battle
// 0x08B3FCF4 and 0x08B4157C), so every call goes through dispatch and meets
// the override; the input functions themselves are called inline from the
// same generated unit and could not be overridden.
struct Install {
    Install() {
        const Options &o = options();
        // The free input only through the decompiled free update, which
        // calls it by lookup; the original update calls it inline.
        const bool follow = follow_zone_on() || o.trace_follow;
        std::vector<decomp::Override> overrides;
        if (o.enabled) {
            overrides.push_back({F_FREE_UPDATE, "camera_free_update", decomp::Module::BattleOverlay,
                                 decomp::Returns::Nothing, &run_free, true, nullptr,
                                 o.verify_hook ? nullptr : &check_free});
            overrides.push_back({F_LOCK_UPDATE, "camera_lock_update", decomp::Module::BattleOverlay,
                                 decomp::Returns::Nothing, &run_lock, true, nullptr,
                                 o.verify_hook ? nullptr : &check_lock});
        } else if (follow) {
            overrides.push_back({F_FREE_UPDATE, "camera_free_update", decomp::Module::BattleOverlay,
                                 decomp::Returns::Nothing, &check_free});
        }
        if (follow) {
            overrides.push_back({F_FREE_INPUT, "camera_free_input", decomp::Module::BattleOverlay,
                                 decomp::Returns::Nothing, &run_free_input, true, nullptr, &check_free_input});
            if (follow_zone_on())
                std::printf("[camera] follow dead zone %.1f degrees each side of straight ahead "
                            "(D012_CAMERA_FOLLOW_DEADZONE)\n",
                            o.follow_dead_zone);
        }
        decomp::add_overrides(overrides.data(), overrides.size(), !overrides.empty());
    }
} install_overrides;

} // namespace

bool enabled() { return options().enabled; }

void add_mouse(std::uint8_t &x, std::uint8_t &y) {
    namespace pk = portablekit::camera;
    const Options &o = options();
    if (!o.mouse) return;
    Mouse &m = mouse();
    // What was carried is for the next update, not for one after a menu or a
    // load.
    const auto now = std::chrono::steady_clock::now();
    if (now - m.last > std::chrono::milliseconds(250)) m.yaw = m.pitch = 0.0f;
    m.last = now;
    const pk::Turn taken = pk::take(pk::Source::Mouse);
    // At most half a second of full pushes waits: a big swipe ends rather
    // than spinning on.
    const float most = m.full * 15.0f;
    m.yaw = std::clamp(m.yaw + taken.yaw_degrees, -most, most);
    m.pitch = std::clamp(m.pitch + taken.pitch_degrees, -most, most);
    if ((m.yaw == 0.0f && m.pitch == 0.0f) || m.full <= 0.0f) return;
    const std::uint8_t stick_x = x, stick_y = y;
    // On top of the stick's own push, as far as a full push; what was not
    // turned stays carried.
    const auto push = [&](std::uint8_t &byte, float &carried) {
        if (carried == 0.0f) return;
        const float stick = stick_axis(byte, o.dead_zone);
        const float wanted = std::clamp(stick + carried / m.full, -1.0f, 1.0f);
        int pushed = stick_byte(wanted, o.dead_zone);
        // Never past what is carried (the byte rounded to the nearest would
        // swing a small remainder from side to side for ever): a step back
        // toward the stick's own byte; stick_axis grows with the byte.
        const int step = carried > 0.0f ? -1 : 1;
        while (pushed != byte && std::fabs(stick_axis(static_cast<std::uint8_t>(pushed), o.dead_zone) - stick) >
                                     std::fabs(wanted - stick))
            pushed += step;
        carried -= (stick_axis(static_cast<std::uint8_t>(pushed), o.dead_zone) - stick) * m.full;
        byte = static_cast<std::uint8_t>(pushed);
    };
    push(x, m.yaw);
    push(y, m.pitch);
    // Less than a step of the stick's bytes turns nothing.
    if (std::fabs(m.yaw) < 0.01f) m.yaw = 0.0f;
    if (std::fabs(m.pitch) < 0.01f) m.pitch = 0.0f;
    if (o.trace)
        std::printf("[camera] read %u mouse %+.2f %+.2f degrees, stick %02X %02X -> %02X %02X, carried %+.2f %+.2f "
                    "(full push %.2f)\n",
                    pad().reads, taken.yaw_degrees, taken.pitch_degrees, stick_x, stick_y, x, y, m.yaw, m.pitch,
                    m.full);
}

void send_mouse(input::Pad &mine) {
    Mouse &m = mouse();
    // The free camera took input lately: its degrees, for the direct turn.
    if (!m.free || std::chrono::steady_clock::now() - m.updated > std::chrono::milliseconds(250)) {
        m.sent_yaw = m.sent_pitch = 0.0f;
        add_mouse(mine.rx, mine.ry);
        return;
    }
    const Turn taken = take_mouse();
    const float yaw = m.sent_yaw + taken.yaw, pitch = m.sent_pitch + taken.pitch;
    mine.mouse_yaw = static_cast<std::int16_t>(std::clamp(std::lround(yaw * 100.0f), -32767L, 32767L));
    mine.mouse_pitch = static_cast<std::int8_t>(std::clamp(std::lround(pitch * 4.0f), -127L, 127L));
    // The rounding's remainder goes with the next frame; past the range, none.
    m.sent_yaw = std::clamp(yaw - mine.mouse_yaw / 100.0f, -0.01f, 0.01f);
    m.sent_pitch = std::clamp(pitch - mine.mouse_pitch / 4.0f, -0.25f, 0.25f);
}

void linked_mouse(int player, const input::Pad &pad) {
    if (player < 0 || player > 1) return;
    linked[player].yaw += pad.mouse_yaw / 100.0f;
    linked[player].pitch += pad.mouse_pitch / 4.0f;
}

void clear_linked_mouse() {
    linked[0] = linked[1] = Linked{};
}

void install(portablekit::HleRegistrar &hle) {
    static bool installed = false;
    if (installed || !enabled()) return;
    installed = true;
    const portablekit::HleFunction previous = hle.bound_function("sceCtrl", kSceCtrlReadBufferPositive);
    if (!previous) {
        std::cerr << "[camera] sceCtrlReadBufferPositive is not bound; the right stick cannot reach the camera\n";
        return;
    }
    const Options &o = options();
    // What an online battle's other copy turns this player's camera with.
    net::battle::set_camera({o.dead_zone, o.sensitivity, o.invert_x, o.invert_y});
    std::cout << "[camera] right-stick battle camera on (dead zone " << o.dead_zone << ", sensitivity "
              << o.sensitivity << (o.invert_x ? ", X inverted" : "") << (o.invert_y ? ", Y inverted" : "")
              << "); R3 recentres" << std::endl;
    hle.bind("sceCtrl", kSceCtrlReadBufferPositive, [previous](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
        const std::uint32_t buffer = ctx.gpr[4];
        const std::uint32_t count = std::clamp<std::uint32_t>(ctx.gpr[5], 1u, 64u);
        previous(rt, ctx);
        Pad &p = pad();
        ++p.reads;
        if (buffer == 0u) return;
        const std::uint32_t last = buffer + (count - 1u) * 16u;
        // The game centres the left stick while HOLD is on; so does this.
        const bool hold = (rt.memory().load32(last + 4u) & kHold) != 0u;
        p.x = hold ? 0x80u : rt.memory().load8(last + 10u);
        p.y = hold ? 0x80u : rt.memory().load8(last + 11u);
        const bool r3 = (input::current().player1.extra & ext::kExtraR3) != 0u;
        const bool live = p.reads - p.live_read <= kLiveReads;
        if (r3 && live) {
            for (std::uint32_t i = 0; i < count; ++i) {
                const std::uint32_t at = buffer + i * 16u + 4u;
                rt.memory().store32(at, rt.memory().load32(at) | kSelect);
            }
            if (!p.r3) std::cout << "[camera] R3: recentre (SELECT)" << std::endl;
        }
        p.r3 = r3;
    });
}

} // namespace d012::camera
