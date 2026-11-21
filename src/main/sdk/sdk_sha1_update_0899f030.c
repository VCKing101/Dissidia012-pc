/* SHA-1: add data to a digest, hashing every complete 64-byte block (built with -O3 like the rest of SHA-1). */
#include "main/sdk_gcc1_types.h"

void func_0893FEE0(unsigned int *block, SdkSha1Context *ctx); /* hash one block of 16 words */

static void sha1_copy(void *dst, const void *src, unsigned int n)
{
    unsigned char *d = dst;
    const unsigned char *s = src;
    unsigned int i;

    for (i = 0; i < n; i++)
        *d++ = *s++;
}

/* The block is hashed as big-endian words. */
static void sha1_swap(unsigned int *w, unsigned int n)
{
    unsigned int i;

    for (i = 0; i < n; i++)
        w[i] = __builtin_allegrex_wsbw(w[i]);
}

int func_0899F030(SdkSha1Context *ctx, const unsigned char *data, unsigned int size)
{
    unsigned int block[16];
    unsigned int n;

    if (ctx == 0 || data == 0)
        return 0x80000103;
    if (ctx->computed != 0)
        return 0x80000021;
    ctx->total += size;
    n = ctx->remains + size;
    while (n >= 64) {
        if (ctx->remains != 0) {
            unsigned int fill = 64 - ctx->remains;

            sha1_copy(block, ctx->buf, ctx->remains);
            sha1_copy((unsigned char *)block + ctx->remains, data, fill);
            data += fill;
            ctx->remains = 0;
        } else {
            sha1_copy(block, data, 64);
            data += 64;
        }
        sha1_swap(block, 16);
        func_0893FEE0(block, ctx);
        n -= 64;
    }
    if (n != 0)
        sha1_copy(ctx->buf + ctx->remains, data, n - ctx->remains);
    ctx->remains = n;
    return 0;
}
