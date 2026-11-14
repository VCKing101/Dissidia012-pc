#include "menu/auto_region1.h"

/* ScreenE75C8: state 4 once func_088AC9A8 reports D_089D712C ready. */
void func_08A2BB30(ScreenE75C8 *self)
{
    int ready;

    ready = func_088AC9A8(D_089D712C);
    if (ready != 0) {
        self->work->state = 4;
    }
}
