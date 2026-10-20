// The decompiled pad functions (port/decomp/pad*.c) as native overrides, and
// the port's analog movement on the world map. See
// docs/port/analog-movement.md. MIT licence; see port/README.md.
//
// On by default: the pad module's dead-zone routine (main 0x08872138) runs
// as decompiled C that does exactly what the game's does and also keeps,
// port-side, the stick as a radial dead zone gives it; the world map
// walker's input (battle 0x08B38A0C) takes that stick's direction in place
// of the eight-way bits. Both are reached from their own unit only because
// the corpus is generated with psp_recomp --dispatch-call for them.
//
// D012_NO_SMOOTH_STICK=1   none of it: the game's own functions.
// D012_STICK_DEAD_ZONE=F   the radial dead zone, a fraction of the nub's range
//                          (default 0.1; the host's own, input.dead_zone, is
//                          applied before it).
// D012_DECOMP_PAD=1        also the pad accessors; with the switch above, the
//                          decompiled dead zone and walk as the game's.
// D012_TRACE_PAD_CALLS=1   the accessors in, each caller and mask logged once.
// D012_TRACE_WALK=1        every walk input logged: stick, direction used,
//                          result, the walker's position.

#include "d012_decomp_harness.hpp"
#include "d012_decomp_pad.hpp"

#include "../decomp/pad_types.h"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

namespace d012::decomp {
namespace {

std::uint32_t a(const CallState &in, int i) { return in.gpr[4 + i]; }
std::uint32_t ra(const CallState &in) { return in.gpr[31]; }

bool flag(const char *name) {
    const char *value = std::getenv(name);
    return value != nullptr && *value != '\0' && std::strcmp(value, "0") != 0;
}

bool smooth() {
    static const bool on = !flag("D012_NO_SMOOTH_STICK");
    return on;
}

float dead_zone() {
    static const float value = [] {
        const char *text = std::getenv("D012_STICK_DEAD_ZONE");
        const float v = text != nullptr ? std::strtof(text, nullptr) : 0.1f;
        return v >= 0.0f && v <= 0.9f ? v : 0.1f;
    }();
    return value;
}

// ---- The stick, port-side ------------------------------------------------------

// What the dead-zone routine last wrote to each stick vector (a RawPad's
// +0x20), and the port's stick from the same bytes.
struct Stick {
    std::array<std::uint8_t, 16> game{};
    PadAnalog smooth{};
};
std::map<gaddr, Stick> &sticks() {
    static std::map<gaddr, Stick> m;
    return m;
}

void run_dead_zone(Guest *g, const CallState &in, GuestRegs &out) {
    std::uint32_t lo = 0, hi = 0;
    pad_apply_dead_zone(g, a(in, 0), a(in, 1), &lo, &hi);
    out.v0 = lo;
    out.v1 = hi;
}

GuestRegs args_of(const CallState &in) {
    GuestRegs r{};
    for (int i = 0; i < 4; ++i) r.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) r.a[4 + i] = in.gpr[8 + i];
    for (int i = 0; i < 4; ++i) r.f[i] = in.fpr[12 + i];
    return r;
}

// Whether the stick bytes a dead-zone call gets are off centre (a pad with
// no stick gets 127.5, one at rest 128).
bool stick_pushed(Guest *g, gaddr in) {
    float x, y;
    std::memcpy(&x, g_ptr(g, in, 4), 4);
    std::memcpy(&y, g_ptr(g, in + 4u, 4), 4);
    return std::fabs(x - 127.5f) > 1.0f || std::fabs(y - 127.5f) > 1.0f;
}

// The game's routine, and the port's stick kept beside it. The differential
// test checks the first calls (at boot, the stick at rest) and, as a probe
// of their own, calls with the stick pushed.
void run_dead_zone_smooth(Guest *g, const CallState &in, GuestRegs &out) {
    if (verifying() && stick_pushed(g, a(in, 1)))
        verify_probe("pad_apply_dead_zone (stick pushed)", F_PAD_APPLY_DEAD_ZONE, Returns::Integer64, run_dead_zone, in,
                     args_of(in));
    run_dead_zone(g, in, out);
    Stick &s = sticks()[a(in, 0)];
    std::memcpy(s.game.data(), g_ptr(g, a(in, 0), 16), 16);
    s.smooth = pad_smooth_stick(g, a(in, 1), dead_zone());
}

// The port's stick for a pad, when it is the one that moves the walker:
// a LogicalPad, not muted, its walk bits taken from the stick's direction
// bits, and its stick vector the one the dead-zone routine last made for its
// source pad (so the stick is this frame's).
const char *g_walk_refusal = "";

bool battle_fighter(Guest *g, gaddr walker) {
    const gaddr battle = g_ld32(g, G_BATTLE);
    return battle != 0u &&
           (g_ld32(g, battle + BATTLE_FIGHTER0) == walker || g_ld32(g, battle + BATTLE_FIGHTER1) == walker);
}

bool in_battle_too() {
    static const bool on = flag("D012_SMOOTH_STICK_BATTLE");
    return on;
}

bool walk_stick(Guest *g, gaddr walker, gaddr pad, PadAnalog &analog) {
    if (pad_muted(g)) return g_walk_refusal = "muted", false;
    if (!pad_moves_on_stick(g, pad, G_WM_MASK_UP, G_WM_MASK_DOWN, G_WM_MASK_LEFT, G_WM_MASK_RIGHT))
        return g_walk_refusal = "walk not on the stick", false;
    const gaddr src = g_ld32(g, pad + LOGICALPAD_SRC);
    const auto found = sticks().find(src + PAD_STICK);
    if (found == sticks().end()) {
        // The dead-zone routine is called only from its own unit: without
        // psp_recomp --dispatch-call for it the override never runs.
        static bool warned = false;
        if (sticks().empty() && !warned) {
            warned = true;
            std::fprintf(stderr, "[pad] smooth stick: the dead-zone routine (main 0x08872138) never reached its "
                                 "override; regenerate the main corpus with psp_recomp --dispatch-call 0x08872138 "
                                 "(docs/port/analog-movement.md). Walking stays eight-way.\n");
        }
        return g_walk_refusal = "no stick", false;
    }
    if (std::memcmp(found->second.game.data(), g_ptr(g, pad + PAD_STICK, 16), 16) != 0)
        return g_walk_refusal = "stale stick", false;
    analog = found->second.smooth;
    // A battle's replay keeps only the eight-way bits: a fighter steered by
    // the stick's own angle would go elsewhere when the replay is played.
    if (!in_battle_too() && battle_fighter(g, walker)) return g_walk_refusal = "battle", false;
    g_walk_refusal = "";
    return true;
}

// The last walk input of each walker: whether it took the port's stick, for
// the camera's follow (d012_camera.cpp).
struct Walk {
    bool analog = false;
    PadAnalog stick{};
};
std::map<gaddr, Walk> &walks() {
    static std::map<gaddr, Walk> m;
    return m;
}

// ---- D012_TRACE_WALK -------------------------------------------------------------

bool tracing_walk() {
    static const bool on = flag("D012_TRACE_WALK");
    return on;
}

float ldf(Guest *g, gaddr address) {
    float v;
    std::memcpy(&v, g_ptr(g, address, 4), 4);
    return v;
}

constexpr float kDegrees = 57.29577951f;

void trace_walk(Guest *g, const CallState &in, bool used, const PadAnalog &analog) {
    static std::uint64_t calls = 0;
    if (++calls == 1u) {
        // Once: the pad's action map and the masks it is matched with.
        const gaddr pad = a(in, 2);
        std::printf("[walk] pad %08X vtable %08X map %08X; stick bits up %08X:%08X down %08X:%08X left %08X:%08X "
                    "right %08X:%08X; walk masks up %08X:%08X down %08X:%08X left %08X:%08X right %08X:%08X\n",
                    pad, g_ld32(g, pad + PAD_VTABLE), g_ld32(g, pad + LOGICALPAD_ACTION_MAP),
                    g_ld32(g, G_STICK_BIT_UP + 4u), g_ld32(g, G_STICK_BIT_UP), g_ld32(g, G_STICK_BIT_DOWN + 4u),
                    g_ld32(g, G_STICK_BIT_DOWN), g_ld32(g, G_STICK_BIT_LEFT + 4u), g_ld32(g, G_STICK_BIT_LEFT),
                    g_ld32(g, G_STICK_BIT_RIGHT + 4u), g_ld32(g, G_STICK_BIT_RIGHT), g_ld32(g, G_WM_MASK_UP + 4u),
                    g_ld32(g, G_WM_MASK_UP), g_ld32(g, G_WM_MASK_DOWN + 4u), g_ld32(g, G_WM_MASK_DOWN),
                    g_ld32(g, G_WM_MASK_LEFT + 4u), g_ld32(g, G_WM_MASK_LEFT), g_ld32(g, G_WM_MASK_RIGHT + 4u),
                    g_ld32(g, G_WM_MASK_RIGHT));
        const gaddr map = g_ld32(g, pad + LOGICALPAD_ACTION_MAP);
        for (gaddr row = map; map != 0u && row < map + 0x200u; row += 16u) {
            std::printf("[walk]   row phys %08X:%08X -> logical %08X:%08X\n", g_ld32(g, row + 4u), g_ld32(g, row),
                        g_ld32(g, row + 12u), g_ld32(g, row + 8u));
            if (g_ld32(g, row) == g_ld32(g, G_PAD_NO_BITS) && g_ld32(g, row + 4u) == g_ld32(g, G_PAD_NO_BITS + 4u))
                break;
        }
    }
    const gaddr out = a(in, 0), walker = a(in, 1), pad = a(in, 2);
    const gaddr src = g_ld32(g, pad + LOGICALPAD_SRC);
    const float ox = ldf(g, out), oz = ldf(g, out + 8u);
    const bool moving = ox != 0.0f || oz != 0.0f;
    static bool was_moving = false;
    // Every moving frame, and the first still one after.
    if (!moving && !was_moving) return;
    was_moving = moving;
    const float sx = ldf(g, src + PAD_STICK), sy = ldf(g, src + PAD_STICK + 4u);
    // The camera-space direction the walk took: the port's stick, or the
    // eight-way bits the function left in the walker's flags.
    float ix = analog.x, iy = analog.y;
    if (!used) {
        const std::uint8_t f0 = g_ld8(g, walker + WALKER_INPUT_FLAGS), f1 = g_ld8(g, walker + WALKER_INPUT_FLAGS + 1u);
        ix = (f1 & 2u) ? 1.0f : ((f1 & 1u) ? -1.0f : 0.0f);
        iy = (f0 & 0x80u) ? 1.0f : ((f0 & 0x40u) ? -1.0f : 0.0f);
    }
    const float stick_angle = std::atan2(-analog.y, analog.x) * kDegrees;
    std::printf("[walk] %llu stick=%.1f deg %.2f game=(%.3f,%.3f|%.3f) input=%s %.1f deg -> world=(%.4f,%.4f) "
                "heading=%.1f pos=(%.3f,%.3f,%.3f) walker=%08X battle=%08X(%08X,%08X state %u mode %u) %s\n",
                static_cast<unsigned long long>(calls), stick_angle, analog.magnitude, sx, sy,
                ldf(g, src + PAD_STICK + 12u), used ? "port" : "game", std::atan2(-iy, ix) * kDegrees, ox, oz,
                std::atan2(-oz, ox) * kDegrees, ldf(g, walker + 0x30u), ldf(g, walker + 0x34u),
                ldf(g, walker + 0x38u), walker, g_ld32(g, 0x089D6A10u),
                g_ld32(g, 0x089D6A10u) ? g_ld32(g, g_ld32(g, 0x089D6A10u) + 0x38u) : 0u,
                g_ld32(g, 0x089D6A10u) ? g_ld32(g, g_ld32(g, 0x089D6A10u) + 0x3Cu) : 0u,
                g_ld32(g, 0x089D6A10u) ? g_ld32(g, g_ld32(g, 0x089D6A10u) + 0x48u) : 0u,
                g_ld32(g, 0x089D6A10u) ? g_ld32(g, g_ld32(g, 0x089D6A10u) + 0x4Cu) : 0u, g_walk_refusal);
}

void check_walk(Guest *g, const CallState &in, GuestRegs &);

void run_walk(Guest *g, const CallState &in, GuestRegs &) {
    if (verifying()) {
        PadAnalog analog{};
        if (walk_stick(g, a(in, 1), a(in, 2), analog) && analog.magnitude > 0.0f)
            verify_probe("wm_walk_input (stick pushed)", F_WM_WALK_INPUT, Returns::Nothing, check_walk, in,
                         args_of(in));
    }
    wm_walk_input(g, a(in, 0), a(in, 1), a(in, 2), a(in, 3), nullptr);
    walks()[a(in, 1)] = Walk{};
    if (tracing_walk()) {
        PadAnalog analog{};  // shown, not used
        (void)walk_stick(g, a(in, 1), a(in, 2), analog);
        trace_walk(g, in, false, analog);
    }
}

void run_walk_smooth(Guest *g, const CallState &in, GuestRegs &) {
    PadAnalog analog{};
    const bool used = walk_stick(g, a(in, 1), a(in, 2), analog);
    if (verifying() && analog.magnitude > 0.0f)
        verify_probe("wm_walk_input (stick pushed)", F_WM_WALK_INPUT, Returns::Nothing, check_walk, in, args_of(in));
    wm_walk_input(g, a(in, 0), a(in, 1), a(in, 2), a(in, 3), used ? &analog : nullptr);
    walks()[a(in, 1)] = Walk{used, used ? analog : PadAnalog{}};
    if (tracing_walk()) trace_walk(g, in, used && analog.magnitude > 0.0f, analog);
}

// The faithful walk for the differential test: no tracing, no port state.
void check_walk(Guest *g, const CallState &in, GuestRegs &) {
    wm_walk_input(g, a(in, 0), a(in, 1), a(in, 2), a(in, 3), nullptr);
}

// ---- D012_TRACE_PAD_CALLS ----------------------------------------------------------

bool tracing_calls() {
    static const bool on = flag("D012_TRACE_PAD_CALLS");
    return on;
}

const char *field_name(std::uint32_t field) {
    switch (field) {
    case PAD_HELD: return "held";
    case PAD_PRESSED: return "pressed";
    case PAD_RELEASED: return "released";
    default: return "repeat";
    }
}

// Every caller and mask once.
void trace_test(const char *kind, std::uint32_t field, const CallState &in, std::uint32_t result) {
    static std::set<std::tuple<std::string, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t>> seen;
    if (!seen.insert({kind, field, ra(in), a(in, 2), a(in, 3)}).second) return;
    std::printf("[pad] %s %s ra=%08X pad=%08X mask=%08X:%08X -> %u\n", kind, field_name(field), ra(in), a(in, 0),
                a(in, 3), a(in, 2), result);
}

// A caller's stick when it changes, a few thousand lines in all.
void trace_stick(const char *kind, Guest *g, const CallState &in) {
    static std::map<std::uint32_t, std::array<int, 4>> last;
    static int lines = 0;
    float v[4];
    std::memcpy(v, g_ptr(g, a(in, 0), 16), 16);
    std::array<int, 4> q{};
    for (int i = 0; i < 4; ++i) q[i] = static_cast<int>(std::lround(v[i] * 1000.0f));
    auto [it, inserted] = last.try_emplace(ra(in), q);
    if (!inserted && it->second == q) return;
    it->second = q;
    if (++lines > 4000) return;
    std::printf("[pad] %s stick ra=%08X pad=%08X dir=(%.3f,%.3f) len=%.3f\n", kind, ra(in), a(in, 1), v[0], v[1],
                v[3]);
}

template <std::uint32_t Field>
void run_logical_test(Guest *g, const CallState &in, GuestRegs &out) {
    out.v0 = logical_pad_test(g, a(in, 0), Field, a(in, 2), a(in, 3));
    if (tracing_calls()) trace_test("logical", Field, in, out.v0);
}
template <std::uint32_t Field>
void run_base_test(Guest *g, const CallState &in, GuestRegs &out) {
    out.v0 = pad_base_test(g, a(in, 0), Field, a(in, 2), a(in, 3));
    if (tracing_calls()) trace_test("base", Field, in, out.v0);
}
void run_logical_stick(Guest *g, const CallState &in, GuestRegs &) {
    logical_pad_get_stick(g, a(in, 0), a(in, 1));
    if (tracing_calls()) trace_stick("logical", g, in);
}
void run_base_stick(Guest *g, const CallState &in, GuestRegs &) {
    pad_base_get_stick(g, a(in, 0), a(in, 1));
    if (tracing_calls()) trace_stick("base", g, in);
}

const Override kAccessors[] = {
    {0x08871CFCu, "pad_base_held", Module::Main, Returns::Integer, run_base_test<PAD_HELD>},
    {0x08871D18u, "pad_base_pressed", Module::Main, Returns::Integer, run_base_test<PAD_PRESSED>},
    {0x08871D34u, "pad_base_released", Module::Main, Returns::Integer, run_base_test<PAD_RELEASED>},
    {0x08871D50u, "pad_base_repeat", Module::Main, Returns::Integer, run_base_test<PAD_REPEAT>},
    {0x08871D6Cu, "pad_base_get_stick", Module::Main, Returns::Nothing, run_base_stick},
    {0x08872740u, "logical_pad_held", Module::Main, Returns::Integer, run_logical_test<PAD_HELD>},
    {0x0887279Cu, "logical_pad_pressed", Module::Main, Returns::Integer, run_logical_test<PAD_PRESSED>},
    {0x088727F8u, "logical_pad_released", Module::Main, Returns::Integer, run_logical_test<PAD_RELEASED>},
    {0x08872854u, "logical_pad_repeat", Module::Main, Returns::Integer, run_logical_test<PAD_REPEAT>},
    {0x088728B0u, "logical_pad_get_stick", Module::Main, Returns::Nothing, run_logical_stick},
};

// The two the port changes. The dead zone's guest-visible behaviour is the
// game's either way (the port's stick is kept host-side); the walk takes the
// port's stick. The differential test checks the faithful versions.
const Override kSmooth[] = {
    {F_PAD_APPLY_DEAD_ZONE, "pad_apply_dead_zone", Module::Main, Returns::Integer64, run_dead_zone_smooth, true,
     nullptr, run_dead_zone},
    {F_WM_WALK_INPUT, "wm_walk_input", Module::BattleOverlay, Returns::Nothing, run_walk_smooth, true, nullptr,
     check_walk},
};
// The dead zone keeps the port's stick here too, for D012_TRACE_WALK to show.
const Override kFaithful[] = {
    {F_PAD_APPLY_DEAD_ZONE, "pad_apply_dead_zone", Module::Main, Returns::Integer64, run_dead_zone_smooth, true,
     nullptr, run_dead_zone},
    {F_WM_WALK_INPUT, "wm_walk_input", Module::BattleOverlay, Returns::Nothing, run_walk, true, nullptr,
     check_walk},
};

struct Install {
    Install() {
        std::vector<Override> wanted;
        const bool decompiled = flag("D012_DECOMP_PAD");
        if (decompiled || tracing_calls())
            for (const Override &o : kAccessors) wanted.push_back(o);
        if (smooth()) {
            for (const Override &o : kSmooth) wanted.push_back(o);
            std::printf("[pad] smooth stick on: radial dead zone %.2f (D012_NO_SMOOTH_STICK=1 for the game's)\n",
                        dead_zone());
        } else if (decompiled || tracing_walk()) {
            for (const Override &o : kFaithful) wanted.push_back(o);
        }
        add_overrides(wanted.data(), wanted.size(), !wanted.empty());
    }
} install;

} // namespace

bool smooth_stick_on() { return smooth(); }

bool analog_walker(std::uint32_t walker, PadAnalog *stick) {
    const auto found = walks().find(walker);
    if (found == walks().end() || !found->second.analog) return false;
    if (stick != nullptr) *stick = found->second.stick;
    return true;
}

} // namespace d012::decomp
