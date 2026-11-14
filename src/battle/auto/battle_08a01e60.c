#include "battle/auto_region2.h"

/* Part02198 reset. */
void func_08A01E60(Part02198 *self)
{
    self->active = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = -1;
    self->field_10 = -1;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_1C = -1;
}
