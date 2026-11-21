/* GMO texture library (VFPU): Quaternion from three Euler angles in radians, from the half-angle sines and cosines (one rotation order). */
#include "main/agent34_sdk.h"

D012Vec4 *func_089BA360(D012Vec4 *out, const D012Vec4 *angles)
{
    __asm__(VFPU_BEGIN
            "lv.q C000, %1\n"
            "vcst.s S010, VFPU_1_PI\n"
            "vscl.t C000, C000, S010\n"
            "vcos.t C010, C000\n"
            "vsin.t C000, C000\n"
            "vcrs.t C020, C010, C010\n"
            "vcrs.t C030, C000, C000\n"
            "vmul.s S003, S020, S010\n"
            "vmul.s S013, S030, S000\n"
            "vmul.t C020, C020, C000\n"
            "vmul.t C030, C030, C010\n"
            "vadd.s S003, S003, S013\n"
            "vpfxt -x, y, -z, w\n"
            "vadd.t C000, C020, C030\n"
            "sv.q C000, %0\n"
            VFPU_END
            : "=m"(*out) : "m"(*angles));
    return out;
}
