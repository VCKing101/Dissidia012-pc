#include "menu/auto_region3.h"

/* ScreenA3F44: enters state 0x15. */
void func_08A989A8(ScreenA3F44 *self)
{
    func_08A9ACAC(self);
    func_08A98A08(self);
    func_08A9C5A0(self, 0);
    self->work->field_81C4 = 4;
    self->work->state = 0x15;
    func_08A9BEA0(self);
}
