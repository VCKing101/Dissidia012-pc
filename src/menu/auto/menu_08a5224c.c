#include "menu/auto_region2.h"

/* Panel base: releases field_1C through field_20 when both are set. */
void func_08A5224C(Panel *self)
{
    if (self->field_1C != 0 && self->field_20 != 0) {
        func_088825D4(self->field_20, self->field_1C);
        self->field_1C = 0;
        self->field_20 = 0;
    }
}
