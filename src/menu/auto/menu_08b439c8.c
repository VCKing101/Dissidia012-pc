#include "menu/auto_region4.h"

void func_08B439C8(Name439C8 *self)
{
    int i;

    self->field_00 = 0;
    self->field_02 = 0;
    /* clears field_0E[i]; written as self + i + 0xE because retail adds
     * the index to self before the member offset */
    for (i = 0; i < 0x22; i++) {
        ((char *)self + i)[0xE] = 0;
    }
}
