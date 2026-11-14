#include "menu/auto_region1.h"

/* ScreenE75C4: resets fields B54..B5C and F64..F6C and clears flag 8. */
void func_089E2D58(ScreenE75C4 *self)
{
    self->work->field_B54 = 0;
    self->work->field_B58 = 0;
    self->work->field_B5C = -1;
    self->work->field_F64 = 0;
    self->work->field_F68 = 0;
    self->work->field_F6C = 0;
    self->work->flags = self->work->flags & ~8;
}
