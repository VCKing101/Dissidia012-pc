#include "menu/auto_region1.h"

/* ScreenE75C4 step: goes to state 2 once func_089E3768 reports done. */
void func_089E16BC(ScreenE75C4 *self)
{
    WorkE75C4 *work;
    int done;

    work = self->work;
    done = func_089E3768(self, &work->layout, work->handles, work->texts);
    if (done != 0) {
        self->work->state = 2;
    }
}
