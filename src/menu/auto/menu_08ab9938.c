#include "menu/auto_region3.h"

/* ScreenBB9E4: state 4 once func_088AC9A8 reports D_089D712C ready. */
void func_08AB9938(ScreenBB9E4 *self)
{
    int ready;

    ready = func_088AC9A8(D_089D712C);
    if (ready != 0) {
        self->work->state = 4;
    }
}
