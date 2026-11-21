/* Offsets point a1 sideways by `half_width` along the unit normal of
 * the two segment directions (b0 - a0) x (b1 - a1), writing the two edge
 * points as 3-float vectors. The whole body is one asm block in the original
 * too: it reuses $t0 (the a1 argument) as the mfc1/mtv scratch register. */
#include "vfpu.h"

typedef struct RibbonPoint {
    float x, y, z;
} RibbonPoint;

void func_0898D19C(RibbonPoint *left, RibbonPoint *right, const VfpuVec4 *a0, const VfpuVec4 *b0,
                   const VfpuVec4 *a1, const VfpuVec4 *b1, float half_width)
{
    __asm__ volatile(
        "lv.q     C000, %2\n"
        "lv.q     C010, %4\n"
        "lv.q     C020, %3\n"
        "lv.q     C030, %5\n"
        "mfc1     $8, %6\n"
        "mtv      $8, S200\n"
        "vsub.t   C100, C020, C000\n"
        "vsub.t   C110, C030, C010\n"
        "vcrsp.t  C120, C100, C110\n"
        "vdot.t   S700, C120, C120\n"
        "vzero.s  S701\n"
        "vcmp.s   EZ, S700\n"
        "vrsq.s   S700, S700\n"
        "vcmovt.s S700, S701, 0\n"
        "vscl.t   C120[-1:1,-1:1,-1:1], C120, S700\n"
        "vscl.t   C130, C120, S200\n"
        "vsub.t   C210, C010, C130\n"
        "vadd.t   C220, C010, C130\n"
        "sv.s     S210, 0 + %0\n"
        "sv.s     S211, 4 + %0\n"
        "sv.s     S212, 8 + %0\n"
        "sv.s     S220, 0 + %1\n"
        "sv.s     S221, 4 + %1\n"
        "sv.s     S222, 8 + %1\n"
        : "=m"(*left), "=m"(*right)
        : "m"(*a0), "m"(*b0), "m"(*a1), "m"(*b1), "f"(half_width));
}
