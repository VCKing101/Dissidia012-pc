#include "menu/auto_region4.h"

void func_08B4278C(Ids4278C *self)
{
    int i;

    self->field_00 = -1;
    self->field_02 = 0;
    self->field_04 = -1;
    for (i = 0; i < 0x20; i++) {
        self->field_10[i] = -1;
    }
}
