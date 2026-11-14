#include "menu/auto_region1.h"

/* Menu root: clears flag 0x100 and opens notice 0 through D_089D727C. */
void func_08A205E4(MenuRoot *self)
{
    self->work->flags = self->work->flags & ~0x100;
    func_088B1354(D_089D727C, 0, func_08A2044C, 0, 0xA0);
}
