#include "main/auto_region2.h"

/* Deleting destructor of Obj885DAC: releases the handle and the model. */
void func_08885DAC(Obj885DAC *self, unsigned int flags)
{
    if (self != 0) {
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
