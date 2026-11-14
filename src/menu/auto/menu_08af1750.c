#include "menu/auto_region3.h"

/* ScreenB01CF8: resets field_2D24..field_2D2C and field_2DD4; clears flag 4. */
void func_08AF1750(ScreenB01CF8 *self)
{
    self->work->field_2D24 = 0;
    self->work->field_2D28 = 0;
    self->work->field_2D2C = -1;
    self->work->field_2DD4 = 0;
    self->work->flags = self->work->flags & 0xFFFFFFFB;
}
