#include "menu/auto_region3.h"

extern const short D_08AA3964[];

/* ScreenA3F44: plays script D_08AA3964, then enters the next state. */
void func_08A9990C(ScreenA3F44 *self)
{
    WorkA3F44 *work;

    func_088D3A04(&self->work->layout);
    func_088D3910(0x1D, &self->work->layout, D_08AA3964, 0, 1);
    work = self->work;
    if (work->field_8AE0 != 0) {
        func_08AB3908(work->field_8AE0, 0);
        work = self->work;
    }
    work->state = work->next;
}
