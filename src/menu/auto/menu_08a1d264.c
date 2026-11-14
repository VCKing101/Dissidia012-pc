#include "menu/auto_region1.h"

/* Menu root: sets flag 0x100 (a callback; both arguments unused). */
void func_08A1D264(MenuRoot *self, int arg1, int arg2)
{
    self->work->flags = self->work->flags | 0x100;
}
