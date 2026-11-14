#include "menu/auto_region3.h"

/* ScreenC4560: advances field_B08 by 3 and returns that entry of field_A88. */
int func_08ABC89C(ScreenC4560 *self)
{
    self->work->field_B08 = self->work->field_B08 + 3;
    return self->work->field_A88[self->work->field_B08];
}
