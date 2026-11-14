#include "menu/auto_region4.h"

int func_08B2A118(Obj29F3C *self)
{
    Work29F3C *work;
    int done;

    func_08B2A970(self);
    func_08B2AF98(self);
    work = self->work;
    func_08B2C908(self, work->layout, work->handles, work->texts);
    work = self->work;
    done = func_08B2B574(self, work->layout, work->handles, work->texts);
    if (done != 0) {
        self->work->state = 3;
    }
    return 0;
}
