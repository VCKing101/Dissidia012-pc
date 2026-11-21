/* GMO texture library (VFPU): 4x4 matrix product of a and b (vmmul.q E000, E200, E100 with the rows of a in M100, of b in M200). */
#include "main/agent34_sdk.h"

D012Mat4 *func_089BA200(D012Mat4 *out, const D012Mat4 *a, const D012Mat4 *b)
{
    __asm__(VFPU_BEGIN
            "lv.q C100, 0 + %1\n"
            "lv.q C110, 16 + %1\n"
            "lv.q C120, 32 + %1\n"
            "lv.q C130, 48 + %1\n"
            "lv.q C200, 0 + %2\n"
            "lv.q C210, 16 + %2\n"
            "lv.q C220, 32 + %2\n"
            "lv.q C230, 48 + %2\n"
            "vmmul.q E000, E200, E100\n"
            "sv.q C000, 0 + %0\n"
            "sv.q C010, 16 + %0\n"
            "sv.q C020, 32 + %0\n"
            "sv.q C030, 48 + %0\n"
            VFPU_END
            : "=m"(*out) : "m"(*a), "m"(*b));
    return out;
}
