#include "battle/auto_region1.h"

/* UnitFA2C8 deleting destructor. */
void func_089F99AC(UnitFA2C8 *self, int flags)
{
    if (self) {
        self->vtable = D_089FA2C8;
        func_089F9A78(self);
        func_08A081B8(self->field_CC, 2);
        func_08A006E0(self->field_90, 2);
        func_08A01E08(self->field_6C, 2);
        func_08A16790(self->field_3C, 2);
        func_08A017C0(self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
