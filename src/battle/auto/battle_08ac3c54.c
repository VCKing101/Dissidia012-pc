#include "battle/auto_region4.h"

/* Advances the random value D_09B6B824 and returns its next 15-bit value
 * (0..0x7FFF). */
unsigned int func_08AC3C54(void)
{
    D_09B6B824 = D_09B6B824 * 0x343FD + 0x269EC3;
    return D_09B6B824 >> 0x10 & 0x7FFF;
}
