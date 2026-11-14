#include "menu/auto_region1.h"

/* Menu root: clears flag 2, then state 3. */
void func_08A1C5C4(MenuRoot *self)
{
    self->work->flags = self->work->flags & ~2;
    self->work->state = 3;
}
