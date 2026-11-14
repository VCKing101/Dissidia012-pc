#include "main/auto_region4.h"

/* Obj9CC290: releases its members and the model, then resets. */
void func_088DE378(Obj9CC290 *self)
{
    int i;

    func_088DF54C(&self->field_80);
    for (i = 0; i < 2; i++) {
        func_088DED28(&self->field_48[i]);
    }
    func_088DE9D8(&self->field_30);
    func_088DF100(&self->field_14);
    if (self->model != 0) {
        func_088DAB44(self->model);
        self->model = 0;
    }
    func_088DE360(self);
}
