#include "battle/auto_region2.h"

/* Unit15448 deleting destructor. */
void func_08A14654(Unit15448 *self, int flags)
{
    if (self) {
        self->vtable = D_08A15448;
        func_08A14744(self);
        func_08A081B8(&self->field_144, 2);
        func_089F5EB0(&self->field_FC, 2);
        func_089F8248(&self->field_C4, 2);
        func_08A10B00(&self->field_98, 2);
        func_08A01E08(&self->field_74, 2);
        func_08A16790(self->field_44, 2);
        func_08A017C0(&self->field_1C, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
