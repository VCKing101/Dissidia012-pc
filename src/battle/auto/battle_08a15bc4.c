#include "battle/auto_region3.h"

/* Unit165C8 deleting destructor. */
void func_08A15BC4(Unit165C8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A165C8;
        func_08A15C88(self);
        func_08A081B8(&self->field_114, 2);
        func_08A0E3D8(&self->field_7C, 2);
        func_089F8248(&self->field_44, 2);
        func_08A10B00(&self->field_18, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
