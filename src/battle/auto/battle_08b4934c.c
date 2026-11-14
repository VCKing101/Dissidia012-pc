#include "battle/auto_region5.h"

/* Part49220: starts motion arg2 (phase 2) with field_24 set to arg1. */
void func_08B4934C(Part49220 *self, int arg1, int arg2)
{
    self->field_24 = arg1;
    func_088D46E4(&self->field_2C, arg2, 0, 1, 0, -1);
    self->field_2C.flags = self->field_2C.flags | 1;
    func_08B49598(self, self->field_2C.field_04);
    self->field_28 = 0;
    self->phase = 2;
}
