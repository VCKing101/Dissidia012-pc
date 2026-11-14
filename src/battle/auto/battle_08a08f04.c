#include "battle/auto_region2.h"

/* Unit097B0 deleting destructor. */
void func_08A08F04(Unit097B0 *self, int flags)
{
    if (self) {
        self->vtable = D_08A097B0;
        func_08A08FC4(self);
        func_08A083F8(&self->field_9C, 2);
        func_08A088F0(&self->field_6C, 2);
        func_08A16790(self->field_3C, 2);
        func_08A017C0(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
