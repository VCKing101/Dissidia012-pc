#include "menu/auto_region2.h"

/* Panel69A78: fills its prompt. */
void func_08A69A20(Panel69A78 *self)
{
    self->prompt.field_00 = 0;
    self->prompt.field_04 = -1;
    self->prompt.field_08 = -1;
    self->prompt.field_0C = self->field_44[0];
    self->prompt.field_10 = self->handles[0];
    self->prompt.field_14 = self->texts;
    self->prompt.field_18 = 0;
    self->prompt.field_1C = self->messages;
    self->prompt.field_20 = self->field_34;
    self->prompt.field_24 = self->field_64;
    self->prompt.field_28 = 0xFFFF;
    self->prompt.field_2A = 0xFFFF;
}
