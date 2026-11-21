/* Blends the direction at self+0x120 with `v` (each scaled by its own w),
 * renormalises it and keeps the length in its w. The scale and add are the
 * SDK's vector routines (GCC-built library code). */
#include "vfpu.h"

extern "C" {

VfpuVec4 *func_089A3A64(VfpuVec4 *out, const VfpuVec4 *in, float scale);
VfpuVec4 *func_089A38C4(VfpuVec4 *out, const VfpuVec4 *a, const VfpuVec4 *b);

typedef struct DirOwner {
    char pad[0x120];
    VfpuVec4 dir;
} DirOwner;

void func_0883659C(DirOwner *self, VfpuVec4 *v)
{
    VfpuVec4 a;
    VfpuVec4 b;
    float w0 = self->dir.w;
    func_089A3A64(&a, &self->dir, w0);
    float w1 = v->w;
    func_089A3A64(&b, v, w1);
    func_089A38C4(&self->dir, &a, &b);
    self->dir.w = Vfpu::Normalize3(&self->dir);
}

}
