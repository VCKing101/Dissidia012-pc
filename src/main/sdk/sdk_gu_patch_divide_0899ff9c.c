/* Graphics library: patch divide, colour material, specular, texture scale. */
#include "main/sdk_gcc1_types.h"

void func_0899FF9C(int ulevel, int vlevel)
{
    SDK_GU_SEND(0x36, (vlevel << 8) | ulevel);
}

void func_0899FFC8(int components)
{
    SDK_GU_SEND(0x53, components & 7);
}

void func_0899FFF0(float power)
{
    union {
        float f;
        unsigned int i;
    } u;

    u.f = power;
    SDK_GU_SEND(0x5B, u.i >> 8);
}

void func_089A001C(float u, float v)
{
    union {
        float f;
        unsigned int i;
    } a, b;

    a.f = u;
    b.f = v;
    SDK_GU_SEND(0x48, a.i >> 8);
    SDK_GU_SEND(0x49, b.i >> 8);
}
