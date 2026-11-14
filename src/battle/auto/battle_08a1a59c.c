#include "battle/auto_region3.h"

/* Unit1B3F8 deleting destructor. */
void func_08A1A59C(Unit1B3F8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A1B3F8;
        func_08A1A68C(self);
        func_08A081B8(&self->field_138, 2);
        func_089F5EB0(&self->field_F0, 2);
        func_089F8248(&self->field_B8, 2);
        func_08A10B00(&self->field_8C, 2);
        func_08A01E08(&self->field_68, 2);
        func_08A18D80(&self->field_44, 2);
        func_08A017C0(&self->field_1C, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
