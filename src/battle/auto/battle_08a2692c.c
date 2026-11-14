#include "battle/auto_region3.h"

/* Unit27200 deleting destructor. */
void func_08A2692C(Unit27200 *self, int flags)
{
    if (self) {
        self->vtable = D_08A27200;
        func_08A269DC(self);
        func_089F8248(&self->field_50, 2);
        func_08A081B8(&self->field_1C, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
