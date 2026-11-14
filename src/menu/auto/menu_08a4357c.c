#include "menu/auto_region2.h"

/* Screen457F0: work field_408 = 2 once func_088E9934 reports 0. */
void func_08A4357C(Screen457F0 *self)
{
    int busy;

    busy = func_088E9934();
    if (busy == 0) {
        self->work->field_408 = 2;
    }
}
