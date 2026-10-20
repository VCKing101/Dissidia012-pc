/* The battle camera's mode updates (battle overlay 0x08B3F40C, 0x08B40014).
 * Decompiled from ULUS10566 v1.00; behaviourally equivalent, not matching.
 * Each takes a hook that runs between the stock input and the code that
 * builds the view from the mode's state, which is where the port's right
 * stick turns the camera (port/host/d012_camera.cpp). With no hook they are
 * the originals. The free input (0x08B3F7F4) is here too, with the port's
 * angular dead zone for its auto-follow as an option. MIT licence; see
 * port/README.md. */
#include "battle_camera.h"

#include <math.h>

static gaddr camera_row(Guest *g, gaddr camera) {
    return g_ld32(g, G_CAMERA_ROWS) + (uint32_t)g_ld32s(g, camera + CAM_ROW) * ROW_SIZE;
}

/* battle 0x08B3F40C */
void camera_free_update(Guest *g, gaddr mode, gaddr camera, CameraInputHook after_input) {
    g_push(g, 0x10u);  /* the original's frame: the callees see the same $sp */
    const gaddr row = camera_row(g, camera);
    g_call2(g, F_FREE_INPUT, mode, camera);
    if (after_input != 0) after_input(g, mode, camera);
    g_call2(g, F_FREE_LOOKAT, mode, camera);
    g_call2(g, F_FREE_EYE, mode, camera);
    g_call2(g, F_CAMERA_COLLIDE, mode, camera);
    const gaddr owner = g_ld32(g, camera + CAM_OWNER);
    gaddr source = row + ROW_FREE_SPRING;
    if ((g_ld32(g, owner + CHAR_FLAGS_384) & 0x100u) != 0 && g_ld32(g, g_ld32(g, owner + CHAR_PTR_3A4)) != 0)
        source = row + ROW_FREE_SPRING_ALT;
    g_st32(g, camera + CAM_SPRING, g_ld32(g, source));
    g_st32(g, camera + CAM_SPRING + 4u, g_ld32(g, source + 4u));
    g_pop(g, 0x10u);
}

/* battle 0x08B40014 */
void camera_lock_update(Guest *g, gaddr mode, gaddr camera, CameraInputHook after_input) {
    g_push(g, 0x20u);
    const gaddr spring = camera_row(g, camera) + ROW_LOCK_SPRING;
    if (g_ld32s(g, mode + LOCK_STATE) == 1 && g_call2(g, F_LOCK_CHECK_1, mode, camera) == 0) {
        g_st32(g, mode + LOCK_STATE, 0);
    } else if (g_ld32s(g, mode + LOCK_STATE) == 2 && g_call2(g, F_LOCK_CHECK_2, mode, camera) == 0) {
        g_st32(g, mode + LOCK_STATE, 0);
    }
    g_call2(g, F_LOCK_INPUT, mode, camera);
    if (after_input != 0) after_input(g, mode, camera);
    g_call2(g, F_LOCK_EYE, mode, camera);
    g_call2(g, F_LOCK_LOOKAT, mode, camera);
    g_call2(g, F_CAMERA_COLLIDE, mode, camera);
    g_call2(g, F_LOCK_HEIGHT, mode, camera);
    /* The original tests the owner's state here and copies the same two
     * words on both paths; the reads are kept for the same faults. */
    const gaddr owner = g_ld32(g, camera + CAM_OWNER);
    if ((g_ld32(g, owner + CHAR_FLAGS_384) & 0x100u) != 0) (void)g_ld32(g, g_ld32(g, owner + CHAR_PTR_3A4));
    g_st32(g, camera + CAM_SPRING, g_ld32(g, spring));
    g_st32(g, camera + CAM_SPRING + 4u, g_ld32(g, spring + 4u));
    g_pop(g, 0x20u);
}

/* ---- The free camera's input (battle 0x08B3F7F4) ---------------------------- */

static float ldf(Guest *g, gaddr a) {
    float v;
    const uint32_t w = g_ld32(g, a);
    memcpy(&v, &w, 4);
    return v;
}

static float call_f(Guest *g, gaddr fn, float f12, float f13, uint32_t a0) {
    GuestRegs r;
    memset(&r, 0, sizeof r);
    r.a[0] = a0;
    r.f[0] = f12;
    r.f[1] = f13;
    g->call(g, fn, &r);
    return r.f0;
}

/* The free input's wrap of an angle into (-pi, pi), as the game does it:
 * untouched inside, else through fmodf with 2 pi. */
static float wrap_angle(Guest *g, float angle, float pi) {
    if (-pi < angle && !(pi <= angle)) return angle;
    const float two_pi = pi * 2.0f;
    if (!(angle <= 0.0f)) return call_f(g, F_FMODF, pi + angle, two_pi, 0) - pi;
    return pi - call_f(g, F_FMODF, pi - angle, two_pi, 0);
}

static uint32_t held_mask(Guest *g, gaddr pad, gaddr mask) {
    return g_vcall_u64(g, pad, 0x34u, 0x08u, g_ld32(g, mask), g_ld32(g, mask + 4u));
}

/* The port's: the follow sees no angle within the zone of straight ahead.
 * Past its edge the angle it is fed rises from 0 to the follow's largest
 * step `cap` over the ramp (at least `cap` wide), so the turn rate grows
 * without a jump: the game's follow turns by the angle itself up to `cap` a
 * call, which it reaches under 3 degrees, so the zone's angle subtracted
 * alone would be nearly a step. Beyond the ramp it gets the game's own
 * angle, which it caps. Straight back, where the game's follow turns at
 * `cap` already, is left alone. */
float camera_follow_angle(float diff, float cap, const CameraFollowZone *zone) {
    const float away = fabsf(diff);
    if (zone->dead_zone <= 0.0f) return diff;
    if (away <= zone->dead_zone) return 0.0f;
    const float ramp = zone->ramp > cap ? zone->ramp : cap;
    if (away >= zone->dead_zone + ramp) return diff;
    const float fed = cap * (away - zone->dead_zone) / ramp;
    return diff < 0.0f ? -fed : fed;
}

/* battle 0x08B3F7F4 */
void camera_free_input(Guest *g, gaddr mode, gaddr camera, const CameraFollowZone *zone, CameraFollowTrace *trace) {
    if (trace != 0) trace->followed = 0;
    g_push(g, 0x30u);
    if (g_ld8(g, camera + CAM_MANUAL) == 0) {
        g_pop(g, 0x30u);
        return;
    }
    const gaddr row = camera_row(g, camera);
    const gaddr pad = g_ld32(g, camera + CAM_PAD);
    const float dt = call_f(g, F_CAMERA_TIME_STEP, 0.0f, 0.0f, camera);
    const float speed = ldf(g, row + ROW_FREE_YAW_SPEED + (g_ld32(g, camera + CAM_SPEED_INDEX) << 2));
    const float pi = ldf(g, G_FREE_PI);

    /* The two turn actions: a step of the row's speed, wrapped. */
    if (held_mask(g, pad, G_FREE_MASK_TURN_POS)) {
        const float yaw = ldf(g, mode + FREE_YAW) + pi / 180.0f * speed * dt;
        g_stf(g, mode + FREE_YAW, yaw);
        g_stf(g, mode + FREE_YAW, wrap_angle(g, yaw, pi));
    } else if (held_mask(g, pad, G_FREE_MASK_TURN_NEG)) {
        const float yaw = ldf(g, mode + FREE_YAW) - pi / 180.0f * speed * dt;
        g_stf(g, mode + FREE_YAW, yaw);
        g_stf(g, mode + FREE_YAW, wrap_angle(g, yaw, pi));
    }

    /* Closer and further. */
    if (held_mask(g, pad, G_FREE_MASK_CLOSER)) {
        g_stf(g, mode + FREE_DISTANCE, ldf(g, mode + FREE_DISTANCE) - ldf(g, row + ROW_FREE_DISTANCE_SPEED) * dt);
    } else if (held_mask(g, pad, G_FREE_MASK_FURTHER)) {
        g_stf(g, mode + FREE_DISTANCE, ldf(g, mode + FREE_DISTANCE) + ldf(g, row + ROW_FREE_DISTANCE_SPEED) * dt);
    }

    /* The auto-follow: the yaw goes toward the one behind the owner's
     * facing, at most the row's step a call, while the owner moves. */
    const gaddr owner = g_ld32(g, camera + CAM_OWNER);
    if (g_ld8(g, camera + CAM_FOLLOW) == 1u && g_call1(g, F_OWNER_FOLLOWED, owner) != 0 &&
        g_call1(g, F_OWNER_MOVING, owner) == 1u) {
        const gaddr facing = g_ld32(g, camera + CAM_OWNER) + CHAR_FACING;
        const float target = call_f(g, F_ATAN2, -ldf(g, facing), -ldf(g, facing + 8u), 0);
        const float yaw = ldf(g, mode + FREE_YAW);
        const float cap = ldf(g, row + ROW_FREE_FOLLOW_STEP);
        const float diff = wrap_angle(g, target - yaw, pi);
        /* The port's dead zone (camera_follow_angle); NULL: the game's. */
        const float fed = zone != 0 ? camera_follow_angle(diff, cap, zone) : diff;
        float turned;
        if (fed < -cap) turned = yaw + -cap;
        else if (fed <= cap) turned = yaw + fed;
        else turned = yaw + cap;
        turned = wrap_angle(g, turned, pi);
        g_stf(g, mode + FREE_YAW, turned);
        if (trace != 0) {
            trace->followed = 1;
            trace->yaw_in = yaw;
            trace->target = target;
            trace->diff = diff;
            trace->fed = fed;
            trace->yaw_out = turned;
        }
    }

    /* The recentre: the mode's reset puts the camera behind the facing. */
    if (g_vcall_u64(g, pad, 0x34u, 0x10u, g_ld32(g, G_FREE_MASK_RECENTRE), g_ld32(g, G_FREE_MASK_RECENTRE + 4u)) != 0 &&
        g_call1(g, F_OWNER_NO_RECENTRE, owner) == 0) {
        const gaddr vtable = g_ld32(g, mode) + MODE_VT_RESET;
        const int16_t adjust = g_ld16s(g, vtable);
        const gaddr fn = g_ld32(g, vtable + 4u);
        g_call4(g, fn, mode + (uint32_t)(int32_t)adjust, camera, (uint32_t)(int32_t)adjust, fn);
    }

    /* The distance kept within the row's limits. */
    float distance = ldf(g, mode + FREE_DISTANCE);
    const float low = ldf(g, row + ROW_FREE_MIN), high = ldf(g, row + ROW_FREE_MAX);
    if (distance < low) distance = low;
    else if (!(distance <= high)) distance = high;
    g_stf(g, mode + FREE_DISTANCE, distance);
    g_pop(g, 0x30u);
}
