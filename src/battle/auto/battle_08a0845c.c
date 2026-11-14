#include "battle/auto_region2.h"

/* Part08880 reset. */
void func_08A0845C(Part08880 *self)
{
    self->active = 0;
    self->field_04 = 0;
    self->field_28 = -1;
    self->field_2C = -1;
    self->field_30 = 0;
    self->field_34 = 0;
    self->field_38 = 0;
}
