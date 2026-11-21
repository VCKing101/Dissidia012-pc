/* Image library, TGA reader: convert a run of BGR(A) pixels to RGBA. */
#include "main/sdk_gcc1_types.h"

void func_0899E364(unsigned char *dst, const unsigned char *src, int count, int bpp, unsigned char repeat)
{
    int step = repeat ? 0 : bpp;
    int i;

    switch (bpp) {
    case 1:
        for (i = 0; i < count; i++) {
            *dst++ = *src;
            src += step;
        }
        break;
    case 2: {
        unsigned short *out = (unsigned short *)dst;

        /* TGA's 1555 is A1R5G5B5; the GE wants A1B5G5R5: swap red and blue, force alpha. */
        for (i = 0; i < count; i++) {
            unsigned int c = (src[1] << 8) | src[0];
            unsigned int r = __builtin_allegrex_ext(c, 10, 5);
            unsigned int b = (c & 0x1F) << 10;
            *out++ = (c & 0x83E0) | r | b | 0x8000;
            src += step;
        }
        break;
    }
    case 3:
        for (i = 0; i < count; i++) {
            dst[0] = src[2];
            dst[1] = src[1];
            dst[2] = src[0];
            dst[3] = 0xFF;
            src += step;
            dst += 4;
        }
        break;
    case 4:
        for (i = 0; i < count; i++) {
            dst[0] = src[2];
            dst[1] = src[1];
            dst[2] = src[0];
            dst[3] = src[3];
            src += step;
            dst += 4;
        }
        break;
    }
}
