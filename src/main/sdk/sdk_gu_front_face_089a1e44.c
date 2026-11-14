/* Graphics library (sceGu-like). */
#include "main/sdk_gcc1_types.h"

void func_089A1E44(SdkGuList *ctx, int order)
{
    if (!order)
        *ctx->cur++ = (0x9B << 24) | 1;
    else
        *ctx->cur++ = (0x9B << 24) | 0;
}
