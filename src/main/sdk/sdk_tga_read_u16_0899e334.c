/* Image library, TGA reader: little-endian u16 at any alignment. */
#include "main/sdk_gcc1_types.h"

const unsigned char *func_0899E334(const unsigned char *p, unsigned short *out)
{
    if ((unsigned int)p & 1)
        *out = p[0] | (p[1] << 8);
    else
        *out = *(const unsigned short *)p;
    return p + 2;
}
