#include "battle/auto_region3.h"

/* Unit1F628 deleting destructor. */
void func_08A1EC64(Unit1F628 *self, int flags)
{
    if (self) {
        self->vtable = D_08A1F628;
        func_08A1ED38(self);
        func_08A081B8(&self->field_F4, 2);
        func_089F5EB0(&self->field_AC, 2);
        func_089F8248(&self->field_74, 2);
        func_08A10B00(&self->field_48, 2);
        func_08A10B00(&self->field_1C, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
