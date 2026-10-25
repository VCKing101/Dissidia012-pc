/* The battle camera's per-frame mode updates (battle overlay), the fields
 * they and the port's right-stick camera use, and the game's addresses.
 * Evidence: docs/research/battle-camera.md, docs/port/right-stick-camera.md.
 * MIT licence; see port/README.md. */
#ifndef D012_DECOMP_BATTLE_CAMERA_H
#define D012_DECOMP_BATTLE_CAMERA_H

#include "guest.h"

#ifdef __cplusplus
extern "C" {
#endif

/* main: pointers the camera code reads. */
#define G_CAMERA_ROWS 0x089D4798u      /* -> rows of the camr chunk (battle.bin), 0x94 bytes each */
#define G_PAD_MANAGER 0x089D6008u      /* -> the pad manager (0x08C15B90) */
#define G_PLAYER_FLAGS 0x09AFD960u     /* u16 per player, 4 bytes apart: the camera direction options */
#define G_FRAME_STEP_PTR 0x089D69E0u   /* -> object whose +0x18 is the battle's time step */
#define G_FRAME_STEP 0x089D5EACu       /* float: the main loop's time step */

/* Pad manager. */
#define PAD_LOGICAL0 0xC0u             /* logical pad 0, the one gameplay reads for player 1 */
#define PAD_LOGICAL_SIZE 0x50u         /* logical pad 1 follows it: the opponent's */
#define PAD_RAW0_MODE 0x50u            /* s32: 1 when raw pad 0 is fed by ad-hoc or a replay */
#define PAD_FLAGS 0x264u               /* bit 0 replay playback, bit 1 input muted */
#define PAD_FLAG_REPLAY 0x1u
#define PAD_FLAG_MUTED 0x2u

/* Player option flags: the two camera direction options. */
#define PLAYER_INVERT_VERTICAL 0x1u
#define PLAYER_INVERT_HORIZONTAL 0x2u

/* Camera object (0x310 bytes, one per character). */
#define CAM_SPEED_INDEX 0x0F0u         /* s32, the camera-speed option, 0..2 */
#define CAM_OWNER 0x0F8u               /* -> the owner character */
#define CAM_STEP_SOURCE 0x100u         /* s32, which time step the camera uses */
#define CAM_STEP_OWN 0x104u            /* float, the step for source 2 */
#define CAM_ROW 0x11Cu                 /* s32, row in the camr table */
#define CAM_PAD 0x120u                 /* -> the logical pad this camera listens to */
#define CAM_MANUAL 0x126u              /* u8, manual input enabled */
#define CAM_FOLLOW 0x0F4u              /* u8, 1: the free camera follows behind its owner */
#define CAM_SPRING 0x0E0u              /* two floats copied from the row each update */

/* Mode objects. */
#define FREE_DISTANCE 0x04u            /* float, also sets the height */
#define FREE_YAW 0x08u                 /* float, radians */
#define LOCK_DISTANCE 0x04u
#define LOCK_YAW_DELTA 0x14u           /* float, this frame's manual turn, rewritten by the input */
#define LOCK_STATE 0x1Cu               /* s32 */

/* camr row. */
#define ROW_SIZE 0x94u
#define ROW_FREE_MIN 0x00u
#define ROW_FREE_MAX 0x04u
#define ROW_FREE_DISTANCE_SPEED 0x08u
#define ROW_FREE_YAW_SPEED 0x14u       /* three floats, degrees per time-step unit */
#define ROW_FREE_FOLLOW_STEP 0x20u     /* float, the auto-follow's largest turn a call, radians (0.05) */
#define ROW_FREE_SPRING 0x2Cu
#define ROW_FREE_SPRING_ALT 0x3Cu
#define ROW_LOCK_MIN 0x44u
#define ROW_LOCK_MAX 0x48u
#define ROW_LOCK_DISTANCE_SPEED 0x4Cu
#define ROW_LOCK_YAW_SPEED 0x6Cu
#define ROW_LOCK_SPRING 0x78u

/* Owner character. */
#define CHAR_FLAGS_384 0x384u
#define CHAR_PTR_3A4 0x3A4u
#define CHAR_FACING 0x090u             /* vec4, the way the character faces; the follow puts the camera behind it */

/* battle: the functions. */
#define F_FREE_UPDATE 0x08B3F40Cu      /* mode 0 update, vtable 0x08B3FCD8 + 0x1C */
#define F_FREE_INPUT 0x08B3F7F4u       /* (mode, camera): D-pad turn, distance, auto-follow, recentre */
#define F_CAMERA_TIME_STEP 0x08B3B4FCu /* (camera) -> $f0, in 1/60 s */
#define F_OWNER_FOLLOWED 0x0883739Cu   /* main, (char) -> char+0x542 is 0 or 1 */
#define F_OWNER_MOVING 0x08B2F9BCu     /* (char) -> char+0x5B0 bit 3: it moved this frame */
#define F_OWNER_NO_RECENTRE 0x08B34E38u /* (char) -> char+0x5B6 bit 7 */
#define F_FMODF 0x089941C0u            /* main, ($f12, $f13) -> $f0 */
#define F_ATAN2 0x089A457Cu            /* main, (y $f12, x $f13) -> $f0, VFPU */
#define G_FREE_PI 0x08B3FD68u          /* float pi, the free input's */
#define G_FREE_MASK_TURN_POS 0x08B3FD70u /* u64 {lo, hi} logical camera bits, held: yaw += */
#define G_FREE_MASK_TURN_NEG 0x08B3FD78u /* yaw -= */
#define G_FREE_MASK_CLOSER 0x08B3FD80u /* distance -= */
#define G_FREE_MASK_FURTHER 0x08B3FD88u /* distance += */
#define G_FREE_MASK_RECENTRE 0x08B3FD90u /* pressed: the mode's reset (vtable +0x58) */
#define MODE_VT_RESET 0x58u            /* mode vtable slot: s16 this-adjust, then the function */
#define F_FREE_LOOKAT 0x08B3F500u
#define F_FREE_EYE 0x08B3F5A8u
#define F_CAMERA_COLLIDE 0x08B3DE68u
#define F_LOCK_UPDATE 0x08B40014u      /* mode 1 update, vtable 0x08B41560 + 0x1C */
#define F_LOCK_CHECK_1 0x08B40994u
#define F_LOCK_CHECK_2 0x08B40A50u
#define F_LOCK_INPUT 0x08B40290u
#define F_LOCK_EYE 0x08B40CDCu
#define F_LOCK_LOOKAT 0x08B404F8u
#define F_LOCK_HEIGHT 0x08B40644u

/* Called after the stock camera input and before the view is built from the
 * mode's state, with the mode object and the camera. NULL: none, the
 * original's behaviour. */
typedef void (*CameraInputHook)(Guest *g, gaddr mode, gaddr camera);

/* The port's angular dead zone for the free camera's auto-follow
 * (docs/port/analog-movement.md, "Camera follow"): within `dead_zone` of
 * straight ahead the follow is fed no angle; over the next `ramp` the angle
 * it is fed rises to its largest step. Radians. */
typedef struct CameraFollowZone {
    float dead_zone;
    float ramp;
} CameraFollowZone;

/* What one call of the free input did, for tracing; `followed` 0 when the
 * follow's gates kept it from running. */
typedef struct CameraFollowTrace {
    int followed;
    float yaw_in;    /* the yaw before the follow */
    float target;    /* the yaw behind the owner's facing */
    float diff;      /* target - yaw, wrapped: what the game's follow sees */
    float fed;       /* what it was fed (diff with no zone) */
    float yaw_out;   /* the yaw after the follow */
} CameraFollowTrace;

/* The angle the follow is fed for `diff` (wrapped, -pi..pi), with the
 * follow's largest step `cap`: 0 inside the zone, rising from its edge to
 * `cap`, the game's own angle beyond (see the .c file). */
float camera_follow_angle(float diff, float cap, const CameraFollowZone *zone);

/* battle 0x08B3F7F4: the free camera's input. `zone` NULL is the game's;
 * `trace` may be NULL. */
void camera_free_input(Guest *g, gaddr mode, gaddr camera, const CameraFollowZone *zone, CameraFollowTrace *trace);

/* battle 0x08B3F40C: free camera, one frame. */
void camera_free_update(Guest *g, gaddr mode, gaddr camera, CameraInputHook after_input);
/* battle 0x08B40014: lock-on camera, one frame. */
void camera_lock_update(Guest *g, gaddr mode, gaddr camera, CameraInputHook after_input);

#ifdef __cplusplus
}
#endif

#endif
