/* Graphics library (sceGu-like). */
#include "main/sdk_gcc1_types.h"

void func_089A1E44(SdkGuList *ctx, int a);

void func_089A0CF8(int a)
{
    func_089A1E44(D_09AF67E8.list, a);
}

void func_089A20CC(SdkGuList *ctx, int a);

void func_089A0D20(int a)
{
    func_089A20CC(D_09AF67E8.list, a);
}

void func_089A1E70(SdkGuList *ctx, int a, int b, int c, int d);

void func_089A0D48(int a, int b, int c, int d)
{
    func_089A1E70(D_09AF67E8.list, a, b, c, d);
}

void func_089A1F04(SdkGuList *ctx, int a, int b, int c);

void func_089A0D84(int a, int b, int c)
{
    func_089A1F04(D_09AF67E8.list, a, b, c);
}

void func_089A2008(SdkGuList *ctx, int a, int b, int c, int d);

void func_089A0DB8(int a, int b, int c, int d)
{
    func_089A2008(D_09AF67E8.list, a, b, c, d);
}

void func_089A0DF4(int min, int mag)
{
    SDK_GU_SEND(0xC6, (mag << 8) | min);
}

void func_089A0E20(int tfx, int tcc)
{
    SdkGuList *ctx = D_09AF67E8.list;

    ctx->tex_func_alpha = tcc;
    ctx->tex_func = tfx;
    *ctx->cur++ = (0xC9 << 24) | (ctx->tex_func_mode << 16) | (tcc << 8) | tfx;
}

void func_089A0E60(int u, int v)
{
    SDK_GU_SEND(0xC7, (v << 8) | u);
}
