#include "battle/auto_region2.h"

/* Part01DA8 reset. */
void func_08A01818(Part01DA8 *self)
{
    self->active = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    self->field_10 = -1;
    self->field_14 = -1;
    self->field_18 = 0;
    self->field_1C = 0;
    self->field_20 = 0;
}
