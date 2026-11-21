/* Normalises a direction, keeps its length in w and copies it to self+0xE0.
 * func_089A3868 is the SDK's Vector4 copy (GCC-built library code). */
#include "vfpu.h"

extern "C" {

VfpuVec4 *func_089A3868(VfpuVec4 *dst, const VfpuVec4 *src);

void func_08833AD8(char *self, VfpuVec4 *v)
{
    float len[3]; /* 12-byte frame slot; the length sits in the middle word */
    VFPU_NORMALIZE3(*v, len[1]);
    v->w = len[1];
    func_089A3868((VfpuVec4 *)(self + 0xE0), v);
}

}
