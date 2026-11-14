#include "menu/auto_region1.h"

/* Screen15A88: func_08A12FAC(1), two set-up steps, then state 2. */
void func_08A12140(Screen15A88 *self)
{
    func_08A12FAC(self, 1);
    func_08A11F10(self);
    func_08A12034(self);
    self->work->state = 2;
}
