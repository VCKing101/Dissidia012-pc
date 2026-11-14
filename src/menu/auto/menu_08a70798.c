#include "menu/auto_region2.h"

/* Bytes6: 0, 1, 0, then three -1. */
void func_08A70798(Bytes6 *self)
{
    self->field_00 = 0;
    self->field_01 = 1;
    self->field_02 = 0;
    self->field_03 = -1;
    self->field_04 = -1;
    self->field_05 = -1;
}
