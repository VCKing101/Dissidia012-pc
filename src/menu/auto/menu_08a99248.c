#include "menu/auto_region3.h"

extern const short D_08AA3A60[];

/* ScreenA3F44: plays script D_08AA3A60 and enters the next state. */
void func_08A99248(ScreenA3F44 *self)
{
    void *object;

    func_088D3A04(&self->work->layout);
    func_088D3910(0x1D, &self->work->layout, D_08AA3A60, 0, 1);
    self->work->state = self->work->next;
    object = self->work->field_8AE0;
    if (object != 0) {
        func_08AB3908(object, 1);
    }
}
