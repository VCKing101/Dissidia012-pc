/* out = clamp01(color * (in / 128) / 256), per component, with the colour
 * reached through self->color. */
#include "vfpu.h"

typedef struct ColorOwner {
    int field_00;
    int field_04;
    VfpuVec4 *color;
} ColorOwner;

void func_0898B920(ColorOwner *self, VfpuVec4 *out, const VfpuVec4 *in)
{
    VfpuVec4 scale = { 1.0f / 128.0f, 1.0f / 256.0f, 0.0f, 0.0f };
    __asm__ volatile(
        "lv.q     C000, %1\n"
        "lv.s     S010, 0 + %3\n"
        "lv.s     S011, 4 + %3\n"
        "lv.q     C020, %2\n"
        "vscl.q   C030, C000, S010\n"
        "vmul.q   C030, C020, C030\n"
        "vscl.q   C000, C030, S011\n"
        "vsat0.q  C000, C000\n"
        "sv.q     C000, %0\n"
        : "=m"(*out)
        : "m"(*in), "m"(*self->color), "m"(scale));
}
