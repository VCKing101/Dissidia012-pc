#include "menu/auto_region1.h"

/* Menu root: clears flags 0x3004, then state 3. */
void func_08A1C628(MenuRoot *self)
{
    self->work->flags = self->work->flags & 0xFFFFCFFB;
    self->work->state = 3;
}
