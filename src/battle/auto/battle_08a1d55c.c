#include "battle/auto_region3.h"

/* Unit1E058 deleting destructor. */
void func_08A1D55C(Unit1E058 *self, int flags)
{
    if (self) {
        self->vtable = D_08A1E058;
        func_08A1D64C(self);
        func_089FC010(&self->field_158, 2);
        func_089FDE58(&self->field_124, 2);
        func_08A16790(&self->field_F4, 2);
        func_08A017C0(&self->field_CC, 2);
        func_089F5EB0(&self->field_84, 2);
        func_089F8248(&self->field_4C, 2);
        func_08A10B00(&self->field_20, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
