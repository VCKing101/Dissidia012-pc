#include "menu/auto_region1.h"

/* Menu root: clears flags 0x8D80. */
void func_08A1D8C0(MenuRoot *self)
{
    self->work->flags = self->work->flags & 0xFFFF727F;
}
