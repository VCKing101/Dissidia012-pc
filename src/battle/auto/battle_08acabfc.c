#include "battle/auto_region4.h"

/* Loads model 0x202 and stores it in `slot`. */
void func_08ACABFC(void **slot)
{
    void *model;

    model = func_088D0700(D_089D7830, 1, 0x202, 1);
    *slot = model;
}
