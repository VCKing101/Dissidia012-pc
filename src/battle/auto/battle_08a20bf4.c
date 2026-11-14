#include "battle/auto_region3.h"

/* Unit213C0 deleting destructor. */
void func_08A20BF4(Unit213C0 *self, int flags)
{
    if (self) {
        self->vtable = D_08A213C0;
        func_08A20CC0(self);
        func_08A206D8(&self->field_C8, 2);
        func_08A20148(&self->field_90, 2);
        func_08A01E08(&self->field_6C, 2);
        func_08A16790(&self->field_3C, 2);
        func_08A017C0(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
