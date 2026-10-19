/* The battle camera's mode updates (battle overlay 0x08B3F40C, 0x08B40014).
 * Decompiled from ULUS10566 v1.00; behaviourally equivalent, not matching.
 * Each takes a hook that runs between the stock input and the code that
 * builds the view from the mode's state, which is where the port's right
 * stick turns the camera (port/host/d012_camera.cpp). With no hook they are
 * the originals. MIT licence; see port/README.md. */
#include "battle_camera.h"

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
