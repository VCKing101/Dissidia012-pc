#include "menu/auto_region2.h"

/* Screen4FFF4 task callback: releases its block. */
void func_08A4C1A0(void)
{
    func_08882158(D_08A4FFF4->field_00);
    D_08A4FFF4->field_00 = 0;
}
