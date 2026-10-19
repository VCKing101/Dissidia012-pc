/* The pad module's accessors (main 0x08871CFC..0x08871DA0, 0x08872740..
 * 0x08872964, 0x08873084). Decompiled from ULUS10566 v1.00; behaviourally
 * equivalent, not matching. MIT licence; see port/README.md. */
#include "pad_types.h"

#include <math.h>

static gaddr pad_manager(Guest *g) { return g_ld32(g, G_PAD_MANAGER_PTR); }

/* main 0x08873084 */
int pad_muted(Guest *g) { return (g_ld32(g, pad_manager(g) + PADMGR_FLAGS) & 2u) != 0u; }

/* main 0x08871CFC (held), 0x08871D18 (pressed), 0x08871D34 (released),
 * 0x08871D50 (repeat) */
uint32_t pad_base_test(Guest *g, gaddr pad, uint32_t field, uint32_t lo, uint32_t hi) {
    return ((g_ld32(g, pad + field) & lo) | (g_ld32(g, pad + field + 4u) & hi)) != 0u;
}

/* main 0x08872740, 0x0887279C, 0x088727F8, 0x08872854 */
uint32_t logical_pad_test(Guest *g, gaddr pad, uint32_t field, uint32_t lo, uint32_t hi) {
    if (pad_muted(g)) return 0u;
    return pad_base_test(g, pad, field, lo, hi);
}

/* main 0x08871D6C */
void pad_base_get_stick(Guest *g, gaddr out, gaddr pad) {
    for (uint32_t i = 0; i < 16u; i += 4u) g_st32(g, out + i, g_ld32(g, pad + PAD_STICK + i));
}

/* main 0x088728B0 */
void logical_pad_get_stick(Guest *g, gaddr out, gaddr pad) {
    if (pad_muted(g)) {
        g_stf(g, out + 0u, 0.0f);
        g_stf(g, out + 4u, 0.0f);
        g_stf(g, out + 8u, 0.0f);
        g_stf(g, out + 12u, 1.0f);
        return;
    }
    pad_base_get_stick(g, out, pad);
}

static float g_ldf(Guest *g, gaddr a) {
    float v;
    uint32_t w = g_ld32(g, a);
    memcpy(&v, &w, 4);
    return v;
}

/* The VFPU's normalisation as the game writes it (lv.q; vdot.t; vzero.s;
 * vcmp.s ez; vsqrt.s; vrcp.s; vcmovt.s; vpfxd [-1:1] x3, w masked; vscl.t;
 * sv.q) and as the recompiler computes it: xyz scaled by 1 / length (by 0
 * for a zero vector) and clamped to -1..1, w kept. Returns the length. */
float pad_vfpu_normalize3(float v[4]) {
    const float dot = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + 0.0f * 0.0f;
    const float length = fabsf(sqrtf(dot));
    float scale = 1.0f / length;
    if (dot == 0.0f) scale = 0.0f;
    for (int i = 0; i < 3; ++i) v[i] = fminf(1.0f, fmaxf(-1.0f, v[i] * scale));
    return length;
}

/* main 0x08872138 */
void pad_apply_dead_zone(Guest *g, gaddr out, gaddr in, uint32_t *lo, uint32_t *hi) {
    const gaddr mgr = pad_manager(g);
    uint32_t bits_lo = g_ld32(g, G_PAD_NO_BITS), bits_hi = g_ld32(g, G_PAD_NO_BITS + 4u);
    const gaddr frame = g_push(g, 0x30u);
    for (uint32_t axis = 0; axis < 2u; ++axis) {
        float v = g_ldf(g, in + axis * 4u) - g_ldf(g, G_STICK_CENTRE);
        const float dead_zone = g_ldf(g, mgr + PADMGR_DEAD_ZONE);
        if (!(v <= 0.0f)) {
            v = v - dead_zone;
            if (v < 0.0f) {
                v = 0.0f;
            } else {
                const gaddr bit = axis == 0u ? G_STICK_BIT_RIGHT : G_STICK_BIT_DOWN;
                bits_lo |= g_ld32(g, bit);
                bits_hi |= g_ld32(g, bit + 4u);
            }
        } else {
            v = v + dead_zone;
            if (v <= 0.0f) {
                const gaddr bit = axis == 0u ? G_STICK_BIT_LEFT : G_STICK_BIT_UP;
                bits_lo |= g_ld32(g, bit);
                bits_hi |= g_ld32(g, bit + 4u);
            } else {
                v = 0.0f;
            }
        }
        v = v / (g_ldf(g, G_STICK_HALF_RANGE) - g_ldf(g, mgr + PADMGR_DEAD_ZONE));
        g_stf(g, frame + axis * 4u, v);
    }
    g_stf(g, frame + 8u, 0.0f);
    float vec[4] = {g_ldf(g, frame), g_ldf(g, frame + 4u), g_ldf(g, frame + 8u), g_ldf(g, frame + 12u)};
    const float length = pad_vfpu_normalize3(vec);
    g_stf(g, frame + 0x10u, length);
    for (uint32_t i = 0; i < 3u; ++i) g_stf(g, frame + i * 4u, vec[i]);
    g_stf(g, frame + 12u, length <= 1.0f ? length : 1.0f);
    g_call2(g, F_VEC4_COPY, out, frame);
    g_pop(g, 0x30u);
    *lo = bits_lo;
    *hi = bits_hi;
}

PadAnalog pad_smooth_stick(Guest *g, gaddr in, float dead_zone) {
    PadAnalog a = {0.0f, 0.0f, 0.0f};
    /* The nub rests at 0x80, with 128 steps below it and 127 above (as the
     * port's stick fills it, input/stick.hpp): each side to its own end. */
    const float bx = g_ldf(g, in) - 128.0f, by = g_ldf(g, in + 4u) - 128.0f;
    const float x = bx / (bx < 0.0f ? 128.0f : 127.0f), y = by / (by < 0.0f ? 128.0f : 127.0f);
    /* The nub's range is a square (a full push reaches its corners; the
     * port's stick is stretched to it the same way), so how far it is pushed
     * is the longer axis, and the direction is the vector's own. */
    const float reach = fmaxf(fabsf(x), fabsf(y));
    const float length = sqrtf(x * x + y * y);
    if (!(reach > dead_zone) || !(length > 0.0f)) return a;
    a.x = x / length;
    a.y = y / length;
    a.magnitude = fminf(1.0f, (reach - dead_zone) / (1.0f - dead_zone));
    return a;
}

static int row_maps(Guest *g, gaddr table, gaddr logical, gaddr stick_bit) {
    const uint32_t end_lo = g_ld32(g, G_PAD_NO_BITS), end_hi = g_ld32(g, G_PAD_NO_BITS + 4u);
    const uint32_t want_lo = g_ld32(g, logical), want_hi = g_ld32(g, logical + 4u);
    const uint32_t bit_lo = g_ld32(g, stick_bit), bit_hi = g_ld32(g, stick_bit + 4u);
    for (gaddr row = table;; row += sizeof(PadMapRow)) {
        const PadMapRow *r = G_STRUCT(g, PadMapRow, row);
        if (r->phys[0] == end_lo && r->phys[1] == end_hi) return 0;
        if (((r->logical[0] & want_lo) | (r->logical[1] & want_hi)) != 0u && r->phys[0] == bit_lo &&
            r->phys[1] == bit_hi)
            return 1;
    }
}

int pad_moves_on_stick(Guest *g, gaddr pad, gaddr up, gaddr down, gaddr left, gaddr right) {
    if (g_ld32(g, pad + PAD_VTABLE) != G_LOGICALPAD_VTABLE) return 0;
    const gaddr table = g_ld32(g, pad + LOGICALPAD_ACTION_MAP);
    if (table == 0u) return 0;
    return row_maps(g, table, up, G_STICK_BIT_UP) && row_maps(g, table, down, G_STICK_BIT_DOWN) &&
           row_maps(g, table, left, G_STICK_BIT_LEFT) && row_maps(g, table, right, G_STICK_BIT_RIGHT);
}
