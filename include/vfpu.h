/* VFPU helpers shared by the game's vector maths (see build/dc-results/vfpu.md).
 *
 * The game's SNC-compiled code reaches the VFPU only through GCC-style inline
 * assembly with explicit VFPU registers and memory ("m") operands. SNC parses
 * the asm itself, substitutes the addresses (`0($a1)`, `0($sp)`, or
 * `addiu $aN, $sp, off` + `0($aN)` for a stack operand at a nonzero offset) and
 * schedules the surrounding C code, so these macros produce the retail words.
 * They are real VFPU operations that C cannot express, not padding.
 */
#ifndef VFPU_H
#define VFPU_H

typedef struct VfpuVec4 {
    float x, y, z, w;
} VfpuVec4;

typedef struct VfpuMat4 {
    VfpuVec4 x, y, z, w;
} VfpuMat4;

/* r = a * b for 4x4 matrices (rows of a in M000, rows of b in M100, result M200). */
#define VFPU_MAT4_MUL(r, a, b)                                     \
    __asm__ volatile(                                              \
        "lv.q     C000,  0 + %1\n"                                 \
        "lv.q     C010, 16 + %1\n"                                 \
        "lv.q     C020, 32 + %1\n"                                 \
        "lv.q     C030, 48 + %1\n"                                 \
        "lv.q     C100,  0 + %2\n"                                 \
        "lv.q     C110, 16 + %2\n"                                 \
        "lv.q     C120, 32 + %2\n"                                 \
        "lv.q     C130, 48 + %2\n"                                 \
        "vmmul.q  M200, M000, M100\n"                              \
        "sv.q     C200,  0 + %0\n"                                 \
        "sv.q     C210, 16 + %0\n"                                 \
        "sv.q     C220, 32 + %0\n"                                 \
        "sv.q     C230, 48 + %0\n"                                 \
        : "=m"(r)                                                  \
        : "m"(a), "m"(b))

/* r = m * (v.xyz, 1): 4x4 transform of a point (vtfm4 on the transposed rows). */
#define VFPU_TRANSFORM_POINT(r, m, v)                              \
    __asm__ volatile(                                              \
        "lv.q     C000,  0 + %1\n"                                 \
        "lv.q     C010, 16 + %1\n"                                 \
        "lv.q     C020, 32 + %1\n"                                 \
        "lv.q     C030, 48 + %1\n"                                 \
        "lv.q     C100, %2\n"                                      \
        "vone.s   S103\n"                                          \
        "vtfm4.q  C200, E000, C100\n"                              \
        "sv.q     C200, %0\n"                                      \
        : "=m"(r)                                                  \
        : "m"(m), "m"(v))

/* A 16-byte aligned stack object, aligned by hand as the retail code does
 * (`addiu rX, sp, off+15; and rX, rX, -16`); SNC ignores aligned(16) on locals. */
#define VFPU_ALIGNED_LOCAL(type, name)                             \
    unsigned char name##_buf[sizeof(type) + 15];                   \
    type *name = (type *)(((unsigned int)name##_buf + 15) & ~15)

/* Normalise the xyz of a 16-byte vector in place and store the length.
 * A zero vector stays zero (vcmovt keeps 0 when the squared length is 0);
 * each component is clamped to [-1, 1]; w is left as it was. */
#define VFPU_NORMALIZE3(vec, len)                                  \
    __asm__ volatile(                                              \
        "lv.q     C000, %0\n"                                      \
        "vdot.t   S010, C000, C000\n"                              \
        "vzero.s  S011\n"                                          \
        "vcmp.s   EZ, S010\n"                                      \
        "vsqrt.s  S010, S010\n"                                    \
        "sv.s     S010, %1\n"                                      \
        "vrcp.s   S010, S010\n"                                    \
        "vcmovt.s S010, S011, 0\n"                                 \
        "vscl.t   C000[-1:1,-1:1,-1:1], C000, S010\n"              \
        "sv.q     C000, %0\n"                                      \
        : "+m"(vec), "=m"(len))

/* Distance between the xyz of two 16-byte vectors (C000/C010, S010). */
#define VFPU_DISTANCE3(dist, a, b)                                 \
    __asm__ volatile(                                              \
        "lv.q     C000, %1\n"                                      \
        "lv.q     C010, %2\n"                                      \
        "vsub.t   C000, C000, C010\n"                              \
        "vdot.t   S010, C000, C000\n"                              \
        "vsqrt.s  S010, S010\n"                                    \
        "sv.s     S010, %0\n"                                      \
        : "=m"(dist)                                               \
        : "m"(a), "m"(b))

#ifdef __cplusplus
/* In-class helpers: SNC inlines them and discards the out-of-line copy
 * (.gnu.linkonce.t), and a returned value is loaded once into a register. */
struct Vfpu {
    static float Distance3(const VfpuVec4 *a, const VfpuVec4 *b)
    {
        float d;
        VFPU_DISTANCE3(d, *a, *b);
        return d;
    }
    /* Normalises v's xyz in place and returns the old length. */
    static float Normalize3(VfpuVec4 *v)
    {
        float len;
        VFPU_NORMALIZE3(*v, len);
        return len;
    }
    /* Inlined with pointer arguments: a stack `out` at a nonzero offset
     * stays in an argument register that later code reuses. */
    static void TransformPoint(VfpuVec4 *out, const VfpuMat4 *m, const VfpuVec4 *v)
    {
        VFPU_TRANSFORM_POINT(*out, *m, *v);
    }
};
#endif

#endif
