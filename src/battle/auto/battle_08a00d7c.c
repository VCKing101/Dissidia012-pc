#include "battle/auto_region2.h"

/* Unit01680 deleting destructor. */
void func_08A00D7C(Unit01680 *self, int flags)
{
    if (self) {
        self->vtable = D_08A01680;
        func_08A00E48(self);
        func_08A081B8(&self->field_CC, 2);
        func_08A006E0(&self->field_90, 2);
        func_08A01E08(&self->field_6C, 2);
        func_08A16790(self->field_3C, 2);
        func_08A017C0(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
