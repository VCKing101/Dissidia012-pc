/* Transforms the reference point D_089DA350 and `point` by `m`, then hands
 * both results on to func_0898ECC8 (transformed point first). */
#include "vfpu.h"

extern "C" {

extern VfpuVec4 D_089DA350;
void func_0898ECC8(void *self, VfpuVec4 *point, VfpuVec4 *origin, short kind, int arg);

void func_0898EB40(void *self, const VfpuMat4 *m, short kind, int arg, const VfpuVec4 *point)
{
    VfpuVec4 origin;
    VfpuVec4 moved;
    Vfpu::TransformPoint(&origin, m, &D_089DA350);
    Vfpu::TransformPoint(&moved, m, point);
    func_0898ECC8(self, &moved, &origin, kind, arg);
}

}
