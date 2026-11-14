#include "menu/auto_region1.h"

/* ScreenE75C4 step: calls func_088705F0 on D_089D5FB4, then state 0x12. */
void func_089E1B3C(ScreenE75C4 *self)
{
    func_088705F0(D_089D5FB4);
    self->work->state = 0x12;
}
