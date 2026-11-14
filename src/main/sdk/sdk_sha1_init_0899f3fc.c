/* SHA-1: start a digest. */
#include "main/sdk_gcc1_types.h"

int func_0899F3FC(SdkSha1Context *ctx)
{
    if (ctx == 0)
        return 0x80000103;
    ctx->h[0] = 0x67452301;
    ctx->h[1] = 0xEFCDAB89;
    ctx->h[2] = 0x98BADCFE;
    ctx->h[3] = 0x10325476;
    ctx->h[4] = 0xC3D2E1F0;
    ctx->remains = 0;
    ctx->computed = 0;
    ctx->total = 0;
    return 0;
}
