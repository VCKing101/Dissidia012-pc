#include "menu/auto_region1.h"

/* ScreenE75C4: releases field_A24 and frees field_A20 from its heap. */
void func_089E2CF0(ScreenE75C4 *self)
{
    func_0887E77C(D_089D6998, self->work->field_A24);
    self->work->field_A24 = 0;
    func_0885C538(self->heap, self->work->field_A20, 0);
    self->work->field_A20 = 0;
}
