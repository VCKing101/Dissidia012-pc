#include "menu/auto_region3.h"

/* ScreenA3F44: reads record field_7C20 from field_9B0. */
void func_08A9B7E8(ScreenA3F44 *self)
{
    int id;

    id = self->work->field_9B0;
    if (id != 0) {
        func_08B2BB2C(id, &self->work->field_7C20);
        self->work->field_8048 = self->work->field_7C20.field_10;
    }
}
