#include "battle/auto_region2.h"

/* Unit12A88 deleting destructor. */
void func_08A11AE4(Unit12A88 *self, int flags)
{
    if (self) {
        self->vtable = D_08A12A88;
        func_08A11BE0(self);
        func_08A081B8(&self->field_280, 2);
        func_08A116A0(&self->field_130, 2);
        func_089F8248(&self->field_F0, 2);
        func_08A10B00(&self->field_C4, 2);
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
