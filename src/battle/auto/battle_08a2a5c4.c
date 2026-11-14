#include "battle/auto_region3.h"

/* Unit2AC40 deleting destructor. */
void func_08A2A5C4(Unit2AC40 *self, int flags)
{
    if (self) {
        self->base.vtable = D_08A2AC40;
        func_08A2A67C(self);
        func_089F5EB0(&self->field_80, 2);
        func_089F8248(&self->field_48, 2);
        func_08A10B00(&self->field_1C, 2);
        func_089F1304(&self->base, 0);
        if (flags & 1) {
            func_089F1354(self);
        }
    }
}
