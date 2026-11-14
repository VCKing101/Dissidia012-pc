#include "menu/auto_region1.h"

/* Screen15A88: swaps flag 0x100 for 0x600, field_ED0 = 2. */
void func_08A12C44(Screen15A88 *self)
{
    self->work->flags = self->work->flags & ~0x100;
    self->work->flags = self->work->flags | 0x600;
    self->work->field_ED0 = 2;
}
