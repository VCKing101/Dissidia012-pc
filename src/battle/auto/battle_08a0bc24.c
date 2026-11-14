#include "battle/auto_region2.h"

/* Unit0C540 deleting destructor. */
void func_08A0BC24(Unit0C540 *self, int flags)
{
    if (self) {
        self->vtable = D_08A0C540;
        func_08A0BCD0(self);
        func_08A081B8(&self->field_40, 2);
        func_08A10B00(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
