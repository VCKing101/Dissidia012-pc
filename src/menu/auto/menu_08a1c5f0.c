#include "menu/auto_region1.h"

/* Menu root: sets flag 0x10000 when flag 2 is set, then state 3. */
void func_08A1C5F0(MenuRoot *self)
{
    RootWork *work;

    work = self->work;
    if ((work->flags & 2) != 0) {
        work->flags = work->flags | 0x10000;
        work = self->work;
    }
    work->state = 3;
}
