#include "battle/auto_region3.h"

/* Unit1D3F8 deleting destructor. */
void func_08A1C5AC(Unit1D3F8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A1D3F8;
        func_08A1C688(self);
        func_08A081B8(&self->field_F0, 2);
        func_08A10B00(&self->field_C4, 2);
        func_089F8248(&self->field_8C, 2);
        func_08A017C0(&self->field_64, 2);
        func_08A017C0(&self->field_3C, 2);
        func_08A017C0(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
