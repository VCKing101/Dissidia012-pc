#include "battle/auto_region4.h"

/* Random integer from `low` to `high` inclusive. */
int func_08AC3D5C(int low, int high)
{
    D_09B6B824 = D_09B6B824 * 0x343FD + 0x269EC3;
    return (int)(D_09B6B824 >> 0x10 & 0x7FFFU) % ((high - low) + 1) + low;
}
