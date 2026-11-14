#include "battle/auto_region2.h"

/* Unit0A208 deleting destructor. */
void func_08A098FC(Unit0A208 *self, int flags)
{
    if (self) {
        self->vtable = D_08A0A208;
        func_08A099C8(self);
        func_0899372C(self->field_E0, 2, sizeof(Part08388), func_08A081B8, 0, 0);
        func_089F8248(&self->field_A8, 2);
        func_089F86B8(&self->field_18, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
