#include "menu/auto_region1.h"

/* Screen15A88: func_08A12244(0), clears flags 0x6C0, sets 0x100, state 7. */
void func_08A122B4(Screen15A88 *self)
{
    func_08A12244(self, 0);
    self->work->flags = self->work->flags & 0xFFFFF93F;
    self->work->flags = self->work->flags | 0x100;
    self->work->state = 7;
}
