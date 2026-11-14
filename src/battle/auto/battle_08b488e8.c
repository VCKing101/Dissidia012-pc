#include "battle/auto_region5.h"

/* Part488E8 reset. */
void func_08B488E8(Part488E8 *self)
{
    int i;

    self->field_3C = 0;
    self->field_00 = 0;
    self->field_38 = 0;
    self->field_40 = -1;
    self->field_44 = -1;
    self->field_48 = -1;
    for (i = 0; i < 2; i++) {
        self->field_4C[i] = 0;
    }
    self->field_54 = 0;
    self->field_58 = 0;
    self->field_5C = 0;
    self->field_60 = 0;
    self->field_90 = 0;
}
