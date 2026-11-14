#include "menu/auto_region3.h"

extern const short D_08AA3BD8[];

/* ScreenA3F44: enters state 0x19 and plays script D_08AA3BD8. */
void func_08A98D20(ScreenA3F44 *self)
{
    self->work->state = 0x19;
    func_088D3A04(&self->work->layout);
    func_088D3910(0x1D, &self->work->layout, D_08AA3BD8, 0, 1);
    func_08A9AFB0(self);
    func_08A98D88(self);
}
