#include "menu/auto_region2.h"

/* Request6E168: releases field_00. */
void func_08A6E1C4(Request6E168 *self)
{
    func_08A6E450(self->field_00, self->field_04);
    self->field_00 = 0;
}
