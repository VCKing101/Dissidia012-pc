/* GMO texture library (VFPU): out.xy = a.xy + b.xy * a.zw, out.zw = b.zw * a.zw (I: composes two UV offset/scale pairs). */
#include "main/agent34_sdk.h"

D012Vec4 *func_089BA3F0(D012Vec4 *out, const D012Vec4 *a, const D012Vec4 *b)
{
    __asm__(VFPU_BEGIN
            "lv.q C010, %1\n"
            "lv.q C020, %2\n"
            "vpfxt z, w, z, w\n"
            "vmul.q C000, C020, C010\n"
            "vadd.p C000, C000, C010\n"
            "sv.q C000, %0\n"
            VFPU_END
            : "=m"(*out) : "m"(*a), "m"(*b));
    return out;
}
