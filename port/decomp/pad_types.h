/* The pad (controller input) module of Dissidia 012 (ULUS10566 v1.00), main
 * 0x08871B40..0x08873497, and the world map's walk input (battle overlay):
 * the structures and addresses the decompiled pad functions use.
 * docs/research/pad-module.md maps the module; docs/port/analog-movement.md
 * is the port's change to the stick.
 *
 * MIT licence; see port/README.md. */
#ifndef D012_DECOMP_PAD_TYPES_H
#define D012_DECOMP_PAD_TYPES_H

#include "guest.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The pad manager: a static object at 0x08C15B90, reached through the
 * pointer at 0x089D6008. */
#define G_PAD_MANAGER_PTR 0x089D6008u
#define PADMGR_RAW0 0x000u        /* RawPad raw[2], 0x60 bytes each */
#define PADMGR_LOGICAL0 0x0C0u    /* LogicalPad logical[2], 0x50 bytes each */
#define PADMGR_SYSTEM 0x1E0u      /* RawPad system (replay playback) */
#define PADMGR_DEAD_ZONE 0x258u   /* float, 100.0 */
#define PADMGR_FLAGS 0x264u       /* bit 0: replay playback; bit 1: mute */

/* PadBase, the start of every pad. */
#define PAD_HELD 0x00u            /* u64 masks */
#define PAD_PRESSED 0x08u
#define PAD_RELEASED 0x10u
#define PAD_REPEAT 0x18u
#define PAD_STICK 0x20u           /* float[4] {dir x, dir y, 0, length <= 1} */
#define PAD_VTABLE 0x34u

/* LogicalPad */
#define LOGICALPAD_SRC 0x40u      /* RawPad * */
#define LOGICALPAD_ACTION_MAP 0x4Cu /* PadMapRow[] (table B) or NULL */
#define G_LOGICALPAD_VTABLE 0x089C96E8u

/* A row of a mapping table: when every phys bit is held on the raw pad, the
 * logical bits are held on the logical pad. A table ends with the row whose
 * phys is G_PAD_NO_BITS. */
typedef struct PadMapRow {
    uint32_t phys[2];      /* 0x00 u64 {lo, hi} */
    uint32_t logical[2];   /* 0x08 */
} PadMapRow;               /* 0x10 */

/* Read-only data of the dead-zone routine. */
#define G_PAD_NO_BITS 0x089C9A68u       /* u64 {0, 0}: no bits, a table's end */
#define G_STICK_CENTRE 0x089C972Cu      /* float 127.5: a byte's centre */
#define G_STICK_HALF_RANGE 0x089C9730u  /* float 127.5: full deflection */
/* The analog-direction bits it returns, u64 {lo, hi} each. */
#define G_STICK_BIT_RIGHT 0x089C9A98u
#define G_STICK_BIT_DOWN 0x089C9AA0u
#define G_STICK_BIT_LEFT 0x089C9AA8u
#define G_STICK_BIT_UP 0x089C9AB0u

/* Functions (main) */
#define F_PAD_MUTED 0x08873084u          /* PadMgr_isMuted */
#define F_PADBASE_GET_STICK 0x08871D6Cu  /* PadBase_getStick (vtable +0x28) */
#define F_LOGICALPAD_GET_STICK 0x088728B0u
#define F_PAD_APPLY_DEAD_ZONE 0x08872138u
#define F_VEC4_COPY 0x089A3868u          /* (dst, src) */
#define F_VEC4_SET 0x089A383Cu           /* (dst, $f12..$f15) */
#define F_VEC3_NORMALIZE 0x089A3BC8u     /* (dst, src): xyz scaled to length 1, w kept */
#define F_MTX_COPY 0x089A3EFCu           /* (dst, src) */
#define F_MTX_INVERT 0x089A4030u         /* (dst, src) */
#define F_MTX_APPLY 0x089A3F40u          /* (dst vec, matrix, vec) */
#define F_CAMERA_MATRIX 0x0885EA88u      /* (camera) -> matrix */

/* The world map (battle overlay at 0x089DD4C0) */
#define F_WM_WALK_INPUT 0x08B38A0Cu      /* (out vec, walker, pad, camera) */
#define G_WM_MASK_UP 0x08B3ACD0u         /* u64 logical "walk" bits */
#define G_WM_MASK_DOWN 0x08B3ACD8u
#define G_WM_MASK_RIGHT 0x08B3ACE0u
#define G_WM_MASK_LEFT 0x08B3ACE8u
#define G_ZERO_VEC4 0x09AF84C0u
#define WALKER_INPUT_FLAGS 0x5B0u        /* u8[2]: [0] 0x40 up, 0x80 down; [1] 0x1 left, 0x2 right,
                                          * 0x4 any, 0x20 vertical (z goes to -y), 0x40 no camera */
/* The battle (main): *(G_BATTLE) is the battle object, its two fighters at
 * +0x38 and +0x3C (each with its pad at +0x5A0), none on the world map. A
 * battle records the fighters' pads for its replay (main 0x08889628 ->
 * 0x088CA680) as the eight-way bits only. */
#define G_BATTLE 0x089D6A10u
#define BATTLE_FIGHTER0 0x38u
#define BATTLE_FIGHTER1 0x3Cu
#define WALKER_FLAGS_198 0x198u          /* u8: bit 0, a second transform through the walker's matrix */

/* The port's stick for movement (docs/port/analog-movement.md): the stick's
 * direction and how far it is pushed, 0..1 past a radial dead zone. */
typedef struct PadAnalog {
    float x, y;            /* unit direction, x right, y down (the game's axes) */
    float magnitude;       /* 0 inside the dead zone */
} PadAnalog;

/* main 0x08873084: the mute flag (g->flags & 2). */
int pad_muted(Guest *g);
/* main 0x08871CFC, 0x08871D18, 0x08871D34, 0x08871D50: PadBase isHeld,
 * isPressed, isReleased, isRepeat: any bit of the mask in the field. */
uint32_t pad_base_test(Guest *g, gaddr pad, uint32_t field, uint32_t lo, uint32_t hi);
/* main 0x08872740, 0x0887279C, 0x088727F8, 0x08872854: the LogicalPad
 * versions, 0 while muted. */
uint32_t logical_pad_test(Guest *g, gaddr pad, uint32_t field, uint32_t lo, uint32_t hi);
/* main 0x08871D6C (a0 = out, a1 = this): the stick vector. */
void pad_base_get_stick(Guest *g, gaddr out, gaddr pad);
/* main 0x088728B0: the same, {0, 0, 0, 1} while muted. */
void logical_pad_get_stick(Guest *g, gaddr out, gaddr pad);

/* main 0x08872138: the stick bytes `in` (float {x, y}, 0..255) through the
 * per-axis dead zone into `out` {dir x, dir y, 0, min(length, 1)}; returns
 * the analog-direction bits in *lo, *hi. */
void pad_apply_dead_zone(Guest *g, gaddr out, gaddr in, uint32_t *lo, uint32_t *hi);
/* The port's stick from the same bytes: radial dead zone `dead_zone`
 * (fraction of full deflection), the length rescaled to 0..1 past it, the
 * direction kept. */
PadAnalog pad_smooth_stick(Guest *g, gaddr in, float dead_zone);
/* The game's VFPU normalisation of xyz (w kept), as the recompiler computes
 * it; returns the length. */
float pad_vfpu_normalize3(float v[4]);
/* Whether a LogicalPad's action map takes each of the four logical masks
 * (u64 at the four addresses: up, down, left, right) from the stick's
 * matching analog-direction bit. */
int pad_moves_on_stick(Guest *g, gaddr pad, gaddr up, gaddr down, gaddr left, gaddr right);

/* battle 0x08B38A0C: the world map walker's input direction, the camera
 * applied, length 1 (w = 1), or the zero vector. With `analog` (the port's
 * change) the stick's direction is used in place of the four held bits;
 * NULL is the game's own function. */
void wm_walk_input(Guest *g, gaddr out, gaddr walker, gaddr pad, gaddr camera, const PadAnalog *analog);

#ifdef __cplusplus
}
#endif

#endif
