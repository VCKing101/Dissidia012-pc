/* Multiplies the matrix pointed to by D_089DA278 by `m` and passes the
 * product to the GU library (func_089A04F4, matrix type 1). */
#include "vfpu.h"

extern VfpuMat4 *D_089DA278;
void func_089A04F4(int type, const VfpuMat4 *matrix);

void func_0896FB5C(const VfpuMat4 *m)
{
    VFPU_ALIGNED_LOCAL(VfpuMat4, r);
    VFPU_MAT4_MUL(*r, *D_089DA278, *m);
    func_089A04F4(1, r);
}
