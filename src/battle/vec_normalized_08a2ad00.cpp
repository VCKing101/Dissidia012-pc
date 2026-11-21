/* Returns a normalised copy of a vector; the length is computed and dropped. */
#include "vfpu.h"

extern "C" {

VfpuVec4 func_08A2AD00(const VfpuVec4 &v)
{
    VfpuVec4 r = v;
    float len;
    VFPU_NORMALIZE3(r, len);
    return r;
}

}
