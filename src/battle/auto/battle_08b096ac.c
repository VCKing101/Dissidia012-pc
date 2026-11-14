#include "battle/auto_region5.h"

/* Part096AC: releases field_10, the model at field_28 and field_20. */
void func_08B096AC(Part096AC *self)
{
    if (self->field_10 != 0) {
        func_0889F2B8(D_089D6C30);
        func_08890A00(self->field_10);
        self->field_10 = 0;
        self->field_18 = 0;
    }
    if (self->field_28 != 0) {
        func_088A3D14(self->field_28, self->field_2C);
        func_088A3EF8(self->field_28);
        self->field_28 = 0;
        self->field_2C = 0;
    }
    if (self->field_20 != 0) {
        func_088A0D1C(self->field_20);
        func_088A0CD4(self->field_20);
        self->field_20 = 0;
        self->field_24 = 0;
    }
}
