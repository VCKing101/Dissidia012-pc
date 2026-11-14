/* Image library, BMP reader: little-endian fields at any alignment. */
#include "main/sdk_gcc1_types.h"

const unsigned char *func_0899D750(const unsigned char *p, unsigned short *out)
{
    if ((unsigned int)p & 1)
        *out = p[0] | (p[1] << 8);
    else
        *out = *(const unsigned short *)p;
    return p + 2;
}

const unsigned char *func_0899D780(const unsigned char *p, unsigned int *out)
{
    if ((unsigned int)p & 3)
        *out = (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
    else
        *out = *(const unsigned int *)p;
    return p + 4;
}
