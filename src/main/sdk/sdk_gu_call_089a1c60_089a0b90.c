/* Graphics library (sceGu-like). */
#include "main/sdk_gcc1_types.h"

void func_089A1C60(SdkGuList *ctx, int a, int b);

void func_089A0B90(int a, int b)
{
    func_089A1C60(D_09AF67E8.list, a, b);
}

void func_089A1D08(SdkGuList *ctx, int a);

void func_089A0BBC(int a)
{
    func_089A1D08(D_09AF67E8.list, a);
}

void func_089A0BE4(int x, int y)
{
    SDK_GU_SEND(0x4C, x << 4);
    SDK_GU_SEND(0x4D, y << 4);
}
