#include "main/auto_region2.h"

/* Switches to the 333 MHz clock (func_089C5800(333, 333, 166)), remembering the previous mode. */
void func_08860E3C(void)
{
    D_089D5E84 = D_089D5E80;
    D_089D5E80 = 1;
    func_089C5800(0x14d, 0x14d, 0xa6);
}
