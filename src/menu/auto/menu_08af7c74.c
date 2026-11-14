#include "menu/auto_region4.h"

/* ScreenB01CF8: releases field_18 and returns field_14 to the heap. */
void func_08AF7C74(ScreenB01CF8X *self)
{
    func_0887E77C(D_089D6998, self->work->field_18);
    self->work->field_18 = 0;
    func_0885C538(self->heap, self->work->field_14, 0);
    self->work->field_14 = 0;
}
