#include "menu/auto_region1.h"

/* Menu root: sets flag 0x800 (a callback; both arguments unused). */
void func_08A1D280(MenuRoot *self, int arg1, int arg2)
{
    self->work->flags = self->work->flags | 0x800;
}
