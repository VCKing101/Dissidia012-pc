#include "menu/auto_region3.h"

/* ScreenAB7CFC: field_7CC8 for 1, else field_7CC4. */
int func_08AAB63C(ScreenAB7CFC *self, int which)
{
    int value;

    value = self->work->field_7CC4;
    if (which == 1) {
        value = self->work->field_7CC8;
    }
    return value;
}
