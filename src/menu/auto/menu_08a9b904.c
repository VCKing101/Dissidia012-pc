#include "menu/auto_region3.h"

/* ScreenA3F44: reads record field_7D4C from field_9B8. */
void func_08A9B904(ScreenA3F44 *self)
{
    int id;

    id = self->work->field_9B8;
    if (id != 0) {
        func_08B2BB2C(id, &self->work->field_7D4C);
        self->work->field_8048 = self->work->field_7D4C.field_10;
    }
}
