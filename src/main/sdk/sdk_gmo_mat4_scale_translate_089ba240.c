/* GMO texture library (VFPU): Rows 0-2 of m scaled by v.w; row 3 = m transformed (homogeneous) by v.xyz. */
#include "main/agent34_sdk.h"

D012Mat4 *func_089BA240(D012Mat4 *out, const D012Mat4 *m, const D012Vec4 *v)
{
    __asm__(VFPU_BEGIN
            "lv.q C100, 0 + %1\n"
            "lv.q C110, 16 + %1\n"
            "lv.q C120, 32 + %1\n"
            "lv.q C130, 48 + %1\n"
            "lv.q C230, %2\n"
            "vscl.q C000, C100, S233\n"
            "vscl.q C010, C110, S233\n"
            "vscl.q C020, C120, S233\n"
            "vhtfm4.q C030, E100, C230\n"
            "sv.q C000, 0 + %0\n"
            "sv.q C010, 16 + %0\n"
            "sv.q C020, 32 + %0\n"
            "sv.q C030, 48 + %0\n"
            VFPU_END
            : "=m"(*out) : "m"(*m), "m"(*v));
    return out;
}
