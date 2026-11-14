#include "menu/auto_region3.h"

/* ScreenC47C4: stops its task once func_088AC9A8 reports D_089D712C ready. */
void func_08AC60B4(ScreenC47C4 *self)
{
    int ready;

    ready = func_088AC9A8(D_089D712C);
    if (ready != 0) {
        func_08865158(&self->task);
    }
}
