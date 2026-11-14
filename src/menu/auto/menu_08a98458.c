#include "menu/auto_region3.h"

/* ScreenA3F44: enters state 9. */
void func_08A98458(ScreenA3F44 *self)
{
    func_08AA0D48(self, 1);
    self->work->state = 9;
    func_08A984BC(self);
    self->work->field_81C4 = -1;
    func_08A9C5A0(self, 0);
    func_08A9BEA0(self);
}
