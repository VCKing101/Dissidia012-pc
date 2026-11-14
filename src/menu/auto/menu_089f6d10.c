#include "menu/auto_region1.h"

/* Screen0B6A0: func_089F24D8, func_089F652C, flag 0x400000, func_089F6D58. */
void func_089F6D10(Screen0B6A0 *self)
{
    func_089F24D8(self);
    func_089F652C(self);
    self->work->flags = self->work->flags | 0x400000;
    func_089F6D58(self);
}
