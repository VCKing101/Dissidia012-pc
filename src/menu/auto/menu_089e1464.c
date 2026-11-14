#include "menu/auto_region1.h"

/* ScreenE75C4 step: runs func_089E34F8 on the layout; goes to state 4 once
 * func_088AC9A8 reports D_089D712C ready. */
void func_089E1464(ScreenE75C4 *self)
{
    WorkE75C4 *work;
    int ready;

    work = self->work;
    func_089E34F8(self, &work->layout, work->handles, work->texts);
    ready = func_088AC9A8(D_089D712C);
    if (ready != 0) {
        self->work->state = 4;
    }
}
