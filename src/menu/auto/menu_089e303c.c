#include "menu/auto_region1.h"

/* ScreenE75C4: clears its prompt block. */
void func_089E303C(ScreenE75C4 *self)
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
