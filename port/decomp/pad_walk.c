/* The world map walker's input direction (battle overlay 0x08B38A0C), with
 * the port's analog stick as an option. Decompiled from ULUS10566 v1.00;
 * behaviourally equivalent, not matching. MIT licence; see port/README.md.
 *
 * The game builds the direction from four held logical bits (walk up, down,
 * left, right), each a unit step on its axis, so the walker goes one of
 * eight ways, and those bits come from the stick only past the pad module's
 * per-axis dead zone (main 0x08872138: 100 of 127.5). With `analog`, the
 * stick's own direction goes in instead, from the edge of the port's radial
 * dead zone; everything after it (the camera, the normalisation, the
 * walker's own transform) is the game's. See docs/port/analog-movement.md. */
#include "pad_types.h"

#include <math.h>

static float ldf(Guest *g, gaddr a) {
    float v;
    const uint32_t w = g_ld32(g, a);
    memcpy(&v, &w, 4);
    return v;
}

static void copy16(Guest *g, gaddr to, gaddr from) {
    for (uint32_t i = 0; i < 16u; i += 4u) g_st32(g, to + i, g_ld32(g, from + i));
}

/* The VFPU normalisation in place; the length goes to `length_slot`. */
static void normalize_at(Guest *g, gaddr v, gaddr length_slot) {
    float x[4] = {ldf(g, v), ldf(g, v + 4u), ldf(g, v + 8u), ldf(g, v + 12u)};
    const float length = pad_vfpu_normalize3(x);
    g_stf(g, length_slot, length);
    for (uint32_t i = 0; i < 4u; ++i) g_stf(g, v + i * 4u, x[i]);
}

/* pad->isHeld(mask) through its vtable */
static uint32_t held(Guest *g, gaddr pad, gaddr mask) {
    return g_vcall_u64(g, pad, PAD_VTABLE, 0x08u, g_ld32(g, mask), g_ld32(g, mask + 4u));
}

static void vec4_set(Guest *g, gaddr to, float x, float y, float z, float w) {
    GuestRegs r;
    memset(&r, 0, sizeof r);
    r.a[0] = to;
    r.f[0] = x;
    r.f[1] = y;
    r.f[2] = z;
    r.f[3] = w;
    g->call(g, F_VEC4_SET, &r);
}

static void or8(Guest *g, gaddr a, uint8_t bits) { g_st8(g, a, (uint8_t)(g_ld8(g, a) | bits)); }

/* battle 0x08B38A0C */
void wm_walk_input(Guest *g, gaddr out, gaddr walker, gaddr pad, gaddr camera, const PadAnalog *analog) {
    const gaddr sp = g_push(g, 0x150u);
    const gaddr in = sp + 0x40u;   /* camera space: x right, z towards the viewer */
    const gaddr dir = sp + 0x50u;
    const gaddr flags = walker + WALKER_INPUT_FLAGS;
    for (uint32_t i = 0; i < 16u; i += 4u) g_stf(g, in + i, 0.0f);
    g_st8(g, flags, (uint8_t)(g_ld8(g, flags) & ~0xC0u));
    g_st8(g, flags + 1u, (uint8_t)(g_ld8(g, flags + 1u) & ~0x07u));

    int moving = 0;
    if (held(g, pad, G_WM_MASK_RIGHT)) {
        g_stf(g, in, 1.0f);
        moving = 1;
        or8(g, flags + 1u, 0x02u | 0x04u);
    } else if (held(g, pad, G_WM_MASK_LEFT)) {
        g_stf(g, in, -1.0f);
        moving = 1;
        or8(g, flags + 1u, 0x01u | 0x04u);
    }
    if (held(g, pad, G_WM_MASK_UP)) {
        g_stf(g, in + 8u, -1.0f);
        moving = 1;
        or8(g, flags, 0x40u);
        or8(g, flags + 1u, 0x04u);
    } else if (held(g, pad, G_WM_MASK_DOWN)) {
        g_stf(g, in + 8u, 1.0f);
        moving = 1;
        or8(g, flags, 0x80u);
        or8(g, flags + 1u, 0x04u);
    }

    /* The port: the stick's direction, pushed past the radial dead zone.
     * The direction flags are set as the eight-way bits would be for it,
     * each within 67.5 degrees of its axis. */
    if (analog != NULL && analog->magnitude > 0.0f) {
        const float sin_22_5 = 0.38268343f;
        g_st8(g, flags, (uint8_t)(g_ld8(g, flags) & ~0xC0u));
        g_st8(g, flags + 1u, (uint8_t)(g_ld8(g, flags + 1u) & ~0x07u));
        g_stf(g, in, analog->x);
        g_stf(g, in + 8u, analog->y);
        if (analog->x > sin_22_5) or8(g, flags + 1u, 0x02u);
        else if (analog->x < -sin_22_5) or8(g, flags + 1u, 0x01u);
        if (analog->y < -sin_22_5) or8(g, flags, 0x40u);
        else if (analog->y > sin_22_5) or8(g, flags, 0x80u);
        or8(g, flags + 1u, 0x04u);
        moving = 1;
    }

    /* Vertical movement: forward/back becomes up/down. */
    if (g_ld8(g, flags + 1u) & 0x20u) {
        const float z = ldf(g, in + 8u);
        g_stf(g, in + 8u, 0.0f);
        g_stf(g, in + 4u, -z);
    }

    if (!moving) {
        copy16(g, out, G_ZERO_VEC4);
    } else if (g_ld8(g, flags + 1u) & 0x40u) {
        g_call2(g, F_VEC3_NORMALIZE, dir, in);
        g_stf(g, dir + 12u, 1.0f);
        copy16(g, out, dir);
    } else {
        const gaddr matrix = g_call1(g, F_CAMERA_MATRIX, camera);
        g_call2(g, F_MTX_COPY, sp, matrix);
        g_call2(g, F_MTX_INVERT, sp, sp);
        g_call3(g, F_MTX_APPLY, sp + 0xB0u, sp, in);
        copy16(g, sp + 0x70u, sp + 0xB0u);
        copy16(g, sp + 0x60u, sp + 0x70u);
        g_call2(g, F_VEC4_COPY, dir, sp + 0x60u);
        if (!(g_ld8(g, flags + 1u) & 0x20u)) g_stf(g, dir + 4u, 0.0f);
        normalize_at(g, dir, sp + 0x110u);
        g_stf(g, dir + 12u, 1.0f);
        if (g_ld8(g, walker + WALKER_FLAGS_198) & 1u) {
            /* Into the walker's frame, flattened, and back. */
            g_call2(g, F_MTX_COPY, sp, walker);
            vec4_set(g, sp + 0x30u, 0.0f, 0.0f, 0.0f, 1.0f);
            g_call2(g, F_MTX_INVERT, sp, sp);
            g_call3(g, F_MTX_APPLY, sp + 0xD0u, sp, dir);
            copy16(g, sp + 0x90u, sp + 0xD0u);
            copy16(g, sp + 0x80u, sp + 0x90u);
            g_call2(g, F_VEC4_COPY, in, sp + 0x80u);
            g_stf(g, in + 8u, 0.0f);
            g_stf(g, in + 12u, 1.0f);
            normalize_at(g, in, sp + 0x114u);
            g_call2(g, F_MTX_COPY, sp, walker);
            vec4_set(g, sp + 0x30u, 0.0f, 0.0f, 0.0f, 1.0f);
            g_call3(g, F_MTX_APPLY, sp + 0xF0u, sp, in);
            copy16(g, sp + 0xA0u, sp + 0xF0u);
            copy16(g, sp + 0x80u, sp + 0xA0u);
            g_call2(g, F_VEC4_COPY, dir, sp + 0x80u);
            normalize_at(g, dir, sp + 0x118u);
            g_stf(g, dir + 12u, 1.0f);
        }
        copy16(g, out, dir);
    }
    g_pop(g, 0x150u);
}
