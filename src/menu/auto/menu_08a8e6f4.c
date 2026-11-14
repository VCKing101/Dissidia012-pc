#include "menu/auto_region3.h"

/* View8E3B4: clears its prompt block. */
void func_08A8E6F4(View8E3B4 *self)
{
    self->work->prompt.field_00 = 0;
    self->work->prompt.field_04 = -1;
    self->work->prompt.field_08 = -1;
    self->work->prompt.field_0C = 0;
    self->work->prompt.field_10 = 0;
    self->work->prompt.field_14 = 0;
    self->work->prompt.field_18 = 0;
    self->work->prompt.field_1C = 0;
    self->work->prompt.field_20 = 0;
    self->work->prompt.field_24 = 0;
}
