#include "battle/auto_region2.h"

/* Unit005E8 deleting destructor. */
void func_089FFCE4(Unit005E8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A005E8;
        func_089FFDB0(self);
        func_089F5EB0(&self->field_B0, 2);
        func_089F8248(&self->field_78, 2);
        func_08A16790(self->field_48, 2);
        func_08A017C0(&self->field_20, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
