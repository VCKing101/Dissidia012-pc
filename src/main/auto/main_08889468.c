#include "main/auto_region2.h"

/* Whether kind is 1, 2 or 0x10. */
unsigned char func_08889468(void *self, int kind)
{
    return (kind == 1 || kind == 2) || kind == 0x10;
}
