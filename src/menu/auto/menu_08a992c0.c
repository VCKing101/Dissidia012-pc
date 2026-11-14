#include "menu/auto_region3.h"

extern const short D_08AA3A60[];

/* ScreenA3F44: enters state 0x21 and plays script D_08AA3A60. */
void func_08A992C0(ScreenA3F44 *self)
{
    void *object;

    self->work->state = 0x21;
    func_088D3A04(&self->work->layout);
    func_088D3910(0x1D, &self->work->layout, D_08AA3A60, 0, 1);
    object = self->work->field_8AE0;
    if (object != 0) {
        func_08AB3908(object, 1);
    }
    func_08AA161C(self);
    func_08A9CF8C(self);
    func_08A99364(self);
    func_08A9C5A0(self, 0);
    func_08A9BEA0(self);
}
