/* Graphics library (sceGu-like). */
#include "main/sdk_gcc1_types.h"

void func_089A204C(SdkGuList *ctx, int a, int b, int c, int d);

void func_089A1200(int a, int b, int c, int d)
{
    func_089A204C(D_09AF67E8.list, a, b, c, d);
}

void func_089A123C(int func, int ref, int mask)
{
    SDK_GU_SEND(0xDC, (mask << 16) | (ref << 8) | func);
}

void func_089A1270(int fail, int zfail, int zpass)
{
    SDK_GU_SEND(0xDD, (zpass << 16) | (zfail << 8) | fail);
}
