#include "main/auto_region2.h"

/* Steps the random seed (LCG 0x343FD, 0x269EC3) and stores the next value. */
void func_08842308(void)
{
    D_09AF84D0 = D_09AF84D0 * 0x343fd + 0x269ec3;
    D_09AF8B00 = D_09AF84D0 >> 0x10 & 0x7fff;
}
