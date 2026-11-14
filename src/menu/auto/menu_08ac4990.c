#include "menu/auto_region3.h"

/* ScreenC47C4: reads seven rows with func_088F10AC. */
void func_08AC4990(ScreenC47C4 *self)
{
    int i;

    for (i = 0; i < 7; i++) {
        func_088F10AC(i, self->work->field_98C[i]);
    }
}
