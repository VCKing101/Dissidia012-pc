#include "battle/auto_region4.h"

/* Next random value in [0, 1): the 15-bit value times 1/32768. */
float func_08AC3C88(void)
{
    D_09B6B824 = D_09B6B824 * 0x343FD + 0x269EC3;
    return (float)(D_09B6B824 >> 0x10 & 0x7FFF) * 3.0517578e-05f;
}
