#include "menu/auto_region3.h"

/* ScreenA3F44: enters state 0x1B. */
void func_08A98E50(ScreenA3F44 *self)
{
    func_08A9B11C(self);
    func_08A9B0DC(self);
    self->work->state = 0x1B;
}
