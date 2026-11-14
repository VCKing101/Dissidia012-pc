#include "main/sdk_gcc2_types.h"

extern void func_089BA0F4(void *dst, SdkVec4 *v);

void func_089BA11C(void *dst, float x, float y, float z, float w)
{
    SdkVec4 v;

    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    func_089BA0F4(dst, &v);
}
