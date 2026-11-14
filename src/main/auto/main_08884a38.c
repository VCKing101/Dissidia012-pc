#include "main/auto_region2.h"

/* Deleting destructor of Obj884A38: releases both texts, the Obj87A61C, the handle and the model. */
void func_08884A38(Obj884A38 *self, unsigned int flags)
{
    if (self != 0) {
        if (self->text_0C != 0) {
            func_0887BA5C(self->text_0C);
            self->text_0C = 0;
        }
        if (self->text_10 != 0) {
            func_0887BA5C(self->text_10);
            self->text_10 = 0;
        }
        if (self->field_08 != 0) {
            func_0887A644(self->field_08, 3);
            self->field_08 = 0;
        }
        if (self->handle != 0) {
            func_088DA060(self->handle);
            self->handle = 0;
        }
        if (self->model != 0) {
            func_088DAB44(self->model);
            self->model = 0;
        }
        func_08885B14(self, 0);
        if ((flags & 1) != 0) {
            func_08885B5C(self);
        }
    }
}
