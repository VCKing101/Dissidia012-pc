#include "menu/auto_region3.h"

/* ScreenA690: func_08AF510C on D_08B01CF8 with field_95C4, then redraws. */
void func_08AE6028(ScreenA690 *self)
{
    func_08AF510C(D_08B01CF8, self->work->field_95C4);
    func_08AE60D8(self);
}
