#include "menu/auto_region2.h"

/* Obj6ECF4: releases its request, clears field_00. */
void func_08A6ECF4(Obj6ECF4 *self)
{
    func_08A6E1C4(&self->request);
    self->field_00 = 0;
}
