#include "battle/auto_region2.h"

/* Unit0AE30 deleting destructor. */
void func_08A0A3FC(Unit0AE30 *self, int flags)
{
    if (self) {
        self->vtable = D_08A0AE30;
        func_08A0A4B8(self);
        func_08A081B8(&self->field_978, 2);
        func_089F6430(&self->field_40, 2);
        func_08A10B00(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
