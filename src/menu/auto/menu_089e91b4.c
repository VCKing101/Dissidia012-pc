#include "menu/auto_region1.h"

/* ScreenE9D38: field_170 = 5 once func_088AC9A8(D_089D712C) reports 1. */
void func_089E91B4(ScreenE9D38 *self)
{
    int state;

    state = func_088AC9A8(D_089D712C);
    if (state == 1) {
        self->field_170 = 5;
    }
}
