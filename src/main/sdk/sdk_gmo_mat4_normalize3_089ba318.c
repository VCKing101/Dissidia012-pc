/* GMO texture library (VFPU): Rows 0-2 of m divided by their xyz length, row 3 set to (0, 0, 0, 1). */
#include "main/agent34_sdk.h"

D012Mat4 *func_089BA318(D012Mat4 *out, const D012Mat4 *m)
{
    __asm__(VFPU_BEGIN
            "lv.q C100, 0 + %1\n"
            "lv.q C110, 16 + %1\n"
            "lv.q C120, 32 + %1\n"
            "vdot.t S200, C100, C100\n"
            "vdot.t S201, C110, C110\n"
            "vdot.t S202, C120, C120\n"
            "vrsq.t C200, C200\n"
            "vscl.q C000, C100, S200\n"
            "vscl.q C010, C110, S201\n"
            "vscl.q C020, C120, S202\n"
            "vidt.q C030\n"
            "sv.q C000, 0 + %0\n"
            "sv.q C010, 16 + %0\n"
            "sv.q C020, 32 + %0\n"
            "sv.q C030, 48 + %0\n"
            VFPU_END
            : "=m"(*out) : "m"(*m));
    return out;
}
