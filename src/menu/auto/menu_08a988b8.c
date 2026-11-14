#include "menu/auto_region3.h"

/* ScreenA3F44: enters state 0x11. */
void func_08A988B8(ScreenA3F44 *self)
{
    func_08AA1248(self);
    func_08A9ECB8(self);
    func_08A9C5A0(self, 0);
    self->work->field_81C4 = -1;
    self->work->state = 0x11;
    func_08A9BEA0(self);
}
