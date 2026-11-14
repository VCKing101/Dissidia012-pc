#include "battle/auto_region3.h"

/* Unit1EB40 deleting destructor. */
void func_08A1E244(Unit1EB40 *self, int flags)
{
    if (self) {
        self->vtable = D_08A1EB40;
        func_08A1E30C(self);
        func_08A081B8(&self->field_C8, 2);
        func_089F5EB0(&self->field_80, 2);
        func_089F8248(&self->field_48, 2);
        func_08A10B00(&self->field_1C, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
