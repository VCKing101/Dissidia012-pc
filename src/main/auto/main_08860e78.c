#include "main/auto_region2.h"

/* Switches to the 222 MHz clock (func_089C5800(222, 222, 111)), remembering the previous mode. */
void func_08860E78(void)
{
    D_089D5E84 = D_089D5E80;
    D_089D5E80 = 0;
    func_089C5800(0xde, 0xde, 0x6f);
}
