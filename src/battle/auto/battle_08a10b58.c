#include "battle/auto_region2.h"

/* Part10F98 reset. */
void func_08A10B58(Part10F98 *self)
{
    self->active = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = -1;
    self->field_10 = 0;
    self->field_14 = -1;
    self->field_18 = 0;
    self->field_1C = -1;
    self->field_20 = -1;
    self->field_24 = 0;
}
