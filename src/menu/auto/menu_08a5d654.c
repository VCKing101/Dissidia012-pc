#include "menu/auto_region2.h"

extern int D_08A5DDC8;

/* Panel5DB80: selected cell over both pages. */
int func_08A5D654(Panel5DB80 *self)
{
    int base;

    base = 0;
    if (self->field_614.index == 1) {
        base = D_08A5DDC8;
    }
    return base + self->field_5E8.index;
}
