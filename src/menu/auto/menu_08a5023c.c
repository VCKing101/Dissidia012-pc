#include "menu/auto_region2.h"

/* Screen51CBC task callback: releases its block. */
void func_08A5023C(void)
{
    func_08882158(D_08A51CBC->field_08);
    D_08A51CBC->field_08 = 0;
}
