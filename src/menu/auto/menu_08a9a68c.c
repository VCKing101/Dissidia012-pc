#include "menu/auto_region3.h"

/* ScreenA3F44: keeps func_088D5AC8's result for field_81D0. */
void func_08A9A68C(ScreenA3F44 *self)
{
    int result;

    result = func_088D5AC8(self->work->field_81D0, 0x23, 1, 0);
    self->work->field_81CC = result;
}
