#include "menu/auto_region3.h"

extern const short D_08AA3BE0[];

/* ScreenA3F44: plays script D_08AA3BE0 and enters the next state. */
void func_08A99624(ScreenA3F44 *self)
{
    void *object;

    func_088D3A04(&self->work->layout);
    self->work->state = self->work->next;
    func_088D3910(0x1D, &self->work->layout, D_08AA3BE0, 0, 1);
    object = self->work->field_8AE0;
    if (object != 0) {
        func_08AB3908(object, 2);
    }
}
