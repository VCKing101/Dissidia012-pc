#include "battle/auto_region3.h"

/* Unit1C398 deleting destructor. */
void func_08A1B58C(Unit1C398 *self, int flags)
{
    if (self) {
        self->vtable = D_08A1C398;
        func_08A1B66C(self);
        func_08A081B8(&self->field_138, 2);
        func_089F8248(&self->field_100, 2);
        func_08A214B0(&self->field_88, 2);
        func_08A01E08(&self->field_64, 2);
        func_08A18D80(&self->field_40, 2);
        func_08A017C0(&self->field_18, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
