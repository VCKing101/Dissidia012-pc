#include "battle/auto_region4.h"

/* Advances the random seed D_09AF84D0 and stores its next 15-bit value in
 * D_09B6B824. */
void func_08AC3C18(void)
{
    D_09AF84D0 = D_09AF84D0 * 0x343FD + 0x269EC3;
    D_09B6B824 = D_09AF84D0 >> 0x10 & 0x7FFF;
}
