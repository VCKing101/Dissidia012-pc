#include "menu/auto_region1.h"

/* ScreenE75C8: counts up field_B74 and sets flag 0x200. */
void func_08A2B3D4(ScreenE75C8 *self)
{
    self->work->field_B74.field_00 = self->work->field_B74.field_00 + 1;
    self->work->flags = self->work->flags | 0x200;
}
