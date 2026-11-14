#include "menu/auto_region1.h"

/* Screen15A88: sets flag 0x40, clears 0x400. */
void func_08A12CAC(Screen15A88 *self)
{
    self->work->flags = self->work->flags | 0x40;
    self->work->flags = self->work->flags & ~0x400;
}
