/* Graphics library (sceGu-like). */
#include "main/sdk_gcc1_types.h"

void func_089A113C(int func, int value, int mask)
{
    SDK_GU_SEND(0xDB, ((mask & 0xFF) << 16) | ((value & 0xFF) << 8) | func);
}

void func_089A1178(int func)
{
    SDK_GU_SEND(0xDE, func);
}

void func_089A119C(int mask)
{
    SDK_GU_SEND(0xE7, mask);
}
