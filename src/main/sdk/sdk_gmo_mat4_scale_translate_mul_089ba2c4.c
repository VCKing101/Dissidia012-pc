/* GMO texture library (VFPU): func_089BA200(out, a, t) where t is func_089BA240(m, v): m scaled by v.w, translated by m x v.xyz. */
#include "main/agent34_sdk.h"

D012Mat4 *func_089BA2C4(D012Mat4 *out, const D012Mat4 *a, const D012Mat4 *m, const D012Vec4 *v)
{
    __asm__(VFPU_BEGIN
            "lv.q C300, %3\n"
            "lv.q C000, 0 + %2\n"
            "lv.q C010, 16 + %2\n"
            "lv.q C020, 32 + %2\n"
            "lv.q C030, 48 + %2\n"
            "lv.q C100, 0 + %1\n"
            "lv.q C110, 16 + %1\n"
            "lv.q C120, 32 + %1\n"
            "lv.q C130, 48 + %1\n"
            "vhtfm4.q C230, E000, C300\n"
            "vscl.q C200, C000, S303\n"
            "vscl.q C210, C010, S303\n"
            "vscl.q C220, C020, S303\n"
            "vmmul.q E000, E200, E100\n"
            "sv.q C000, 0 + %0\n"
            "sv.q C010, 16 + %0\n"
            "sv.q C020, 32 + %0\n"
            "sv.q C030, 48 + %0\n"
            VFPU_END
            : "=m"(*out) : "m"(*a), "m"(*m), "m"(*v));
    return out;
}
