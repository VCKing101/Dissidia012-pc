#include "main/auto_region4.h"

/* Obj9CC330: releases the handle, then resets. */
void func_088DF54C(Obj9CC330 *self)
{
    func_088DF590(self);
    if (self->handle != 0) {
        func_088DA060(self->handle);
        self->handle = 0;
    }
    func_088DF53C(self);
}
