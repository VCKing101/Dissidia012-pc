#include "menu/auto_region2.h"

/* Screen4FFF4: closes down and moves to state 5. */
void func_08A4C7B8(Screen4FFF4 *self)
{
    func_08A4EDC0();
    func_08A4F924(self);
    func_08A4C4EC(self);
    func_08885220(D_089D69E8);
    self->work->state = 5;
}
