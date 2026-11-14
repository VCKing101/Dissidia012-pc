/* Graphics library (sceGu-like). */
#include "main/sdk_gcc1_types.h"

void func_089A2008(SdkGuList *ctx, int tpsm, int maxmips, int a2, int swizzle)
{
    *ctx->cur++ = (0xC2 << 24) | (maxmips << 16) | (a2 << 8) | swizzle;
    *ctx->cur++ = (0xC3 << 24) | tpsm;
    *ctx->cur++ = (0xCB << 24) | 0;
}

void func_089A204C(SdkGuList *ctx, int x, int y, int w, int h)
{
    int x1 = x + w - 1;
    int y1 = y + h - 1;

    ctx->scissor_x0 = x;
    ctx->scissor_y0 = y;
    ctx->scissor_x1 = x1;
    ctx->scissor_y1 = y1;
    if (ctx->scissor_enable) {
        *ctx->cur++ = (0xD4 << 24) | (y << 10) | x;
        *ctx->cur++ = (0xD5 << 24) | (y1 << 10) | x1;
        *ctx->cur++ = (0x15 << 24) | 0;
        *ctx->cur++ = (0x16 << 24) | (y1 << 10) | x1;
    }
}

void func_089A20CC(SdkGuList *ctx, int mode)
{
    if (!mode)
        *ctx->cur++ = (0x50 << 24) | 0;
    else
        *ctx->cur++ = (0x50 << 24) | 1;
}
