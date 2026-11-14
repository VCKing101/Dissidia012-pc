#include "menu/auto_region2.h"

/* MenuRoot2: moves to state 3 when func_088AC9A8 reports so. */
void func_08A42678(MenuRoot2 *self)
{
    int done;

    done = func_088AC9A8(D_089D712C);
    if (done != 0) {
        self->work->state = 3;
    }
}
