#include "menu/auto_region2.h"

/* Screen4C0FC: releases work field_BE8 and frees field_BE4. */
void func_08A4ABA0(Screen4C0FC *self)
{
    func_0887E77C(D_089D6998, self->work->field_BE8);
    self->work->field_BE8 = 0;
    func_0885C538(self->heap, self->work->field_BE4, 0);
    self->work->field_BE4 = 0;
}
