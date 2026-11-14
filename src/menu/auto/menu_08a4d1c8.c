#include "menu/auto_region2.h"

/* Screen4FFF4: state 8 and func_08B58C4C(D_08B59230, arg1, arg2, 1). */
void func_08A4D1C8(Screen4FFF4 *self, int arg1, int arg2)
{
    self->work->state = 8;
    func_08B58C4C(D_08B59230, arg1, arg2, 1);
}
