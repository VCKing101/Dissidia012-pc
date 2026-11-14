#include "menu/auto_region4.h"

/* ScreenB01CF8: recounts both totals and returns their sum. */
int func_08AFA880(ScreenB01CF8X *self)
{
    int count;

    count = func_08AF45E8(self);
    self->work->field_C2C = count;
    count = func_08AF47E0(self);
    self->work->field_C30 = count;
    return self->work->field_C2C + self->work->field_C30;
}
