/* d = p2 - ((p0 - p1) * s + p2 * t), out_b = d (w taken from p2),
 * out_a = p0 + d.xyz. The whole body is one asm block in the original too:
 * it reuses $t0 (the p2 argument) as the mfc1/mtv scratch register after
 * loading p2, which compiled C would not do. */
#include "vfpu.h"

void func_08B3B950(VfpuVec4 *out_a, VfpuVec4 *out_b, const VfpuVec4 *p0, const VfpuVec4 *p1,
                   const VfpuVec4 *p2, float s, float t)
{
    __asm__ volatile(
        "lv.q     C000, %2\n"
        "lv.q     C010, %3\n"
        "lv.q     C020, %4\n"
        "mfc1     $8, %5\n"
        "mtv      $8, S030\n"
        "mfc1     $8, %6\n"
        "mtv      $8, S031\n"
        "vsub.t   C100, C000, C010\n"
        "vscl.t   C100, C100, S030\n"
        "vscl.t   C110, C020, S031\n"
        "vadd.t   C100, C100, C110\n"
        "vsub.q   C100, C020, C100[x,y,z,0]\n"
        "vadd.q   C110, C000, C100[x,y,z,0]\n"
        "sv.q     C110, %0\n"
        "sv.q     C100, %1\n"
        : "=m"(*out_a), "=m"(*out_b)
        : "m"(*p0), "m"(*p1), "m"(*p2), "f"(s), "f"(t));
}
