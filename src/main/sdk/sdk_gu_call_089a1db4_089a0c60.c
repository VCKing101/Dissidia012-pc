/* Graphics library (sceGu-like). */
#include "main/sdk_gcc1_types.h"

void func_089A1DB4(SdkGuList *ctx, int a, int b);

void func_089A0C60(int a, int b)
{
    func_089A1DB4(D_09AF67E8.list, a, b);
}

void func_089A0C8C(int b)
{
    SdkGuList *ctx = D_09AF67E8.list;

    func_089A1DB4(ctx, 7, b);
}
