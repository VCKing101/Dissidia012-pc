/* GMO texture library (VFPU): Clamp a colour vector to 0..1 and pack it as one RGBA8888 word. */
#include "main/agent34_sdk.h"

u32 *func_089BA0F4(u32 *out, const D012Vec4 *color)
{
    __asm__(VFPU_BEGIN
            "lv.q C000, %1\n"
            "vsat0.q C000, C000\n"
            "viim.s S010, 255\n"
            "vscl.q C000, C000, S010\n"
            "vf2iz.q C000, C000, 23\n"
            "vi2uc.q S000, C000\n"
            "sv.s S000, %0\n"
            VFPU_END
            : "=m"(*out) : "m"(*color));
    return out;
}
