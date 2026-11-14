/* Graphics library (sceGu-like). */
#include "main/sdk_gcc1_types.h"

void func_089A0F84(float u, float v)
{
    union {
        float f;
        unsigned int i;
    } a, b;

    a.f = u;
    b.f = v;
    SDK_GU_SEND(0x4A, a.i >> 8);
    SDK_GU_SEND(0x4B, b.i >> 8);
}
