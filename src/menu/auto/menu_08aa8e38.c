#include "menu/auto_region3.h"

/* ScreenAB7CFC: once the opening motion ends, enters state 2 (or 7, closing field_7CCC). */
int func_08AA8E38(ScreenAB7CFC *self)
{
    int done;
    WorkAB7CFC *work;

    func_08AA9594(self);
    func_08AA9800(self);
    done = func_08AA99D4(self);
    if (done != 0) {
        work = self->work;
        if ((work->flags & 1) == 0) {
            work->state = 2;
        }
        else {
            work->state = 7;
            func_08AB58E0(self->work->field_7CCC, 6, 1);
            func_08AB5790(self->work->field_7CCC, 2);
        }
    }
    return 0;
}
