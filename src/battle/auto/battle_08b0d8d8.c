#include "battle/auto_region5.h"

/* Part0D8D8: releases the model and field_00 and clears field_0C. */
void func_08B0D8D8(Part0D8D8 *self)
{
    if (self->model != 0) {
        func_088A3D14(self->model, self->motion);
        func_088A3EF8(self->model);
        self->motion = 0;
        self->model = 0;
    }
    if (self->field_00 != 0) {
        func_08890A00(self->field_00);
        self->field_00 = 0;
    }
    self->field_0C = 0;
}
