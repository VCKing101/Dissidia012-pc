#include "menu/auto_region1.h"

/* Screen15A88: sets flag 0x80, clears 0x100, field_ED0 = 3. */
void func_08A12C78(Screen15A88 *self)
{
    self->work->flags = self->work->flags | 0x80;
    self->work->flags = self->work->flags & ~0x100;
    self->work->field_ED0 = 3;
}
