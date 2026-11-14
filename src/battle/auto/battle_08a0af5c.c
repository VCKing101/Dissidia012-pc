#include "battle/auto_region2.h"

/* Unit0BAF0 deleting destructor. */
void func_08A0AF5C(Unit0BAF0 *self, int flags)
{
    if (self) {
        self->vtable = D_08A0BAF0;
        func_08A0B020(self);
        func_08A081B8(&self->field_98, 2);
        func_08A16790(self->field_68, 2);
        func_08A017C0(&self->field_40, 2);
        func_08A10B00(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
