/* Graphics library (sceGu-like). */
#include "main/sdk_gcc1_types.h"

void func_089A0FC8(void)
{
    SDK_GU_SEND(0xCB, 0);
}

void func_089A0FE8(int a, int b, int c, int d)
{
    SDK_GU_SEND(0xC5, (d << 16) | (c << 8) | (b << 2) | a);
}
