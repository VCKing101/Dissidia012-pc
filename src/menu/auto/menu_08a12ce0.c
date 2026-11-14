#include "menu/auto_region1.h"

/* Screen15A88: sets flag 0x100, clears 0x600, field_ED0 = 0. */
void func_08A12CE0(Screen15A88 *self)
{
    self->work->flags = self->work->flags | 0x100;
    self->work->flags = self->work->flags & 0xFFFFF9FF;
    self->work->field_ED0 = 0;
}
