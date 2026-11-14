#include "battle/auto_region2.h"

/* Unit10A10 deleting destructor. */
void func_08A10044(Unit10A10 *self, int flags)
{
    if (self) {
        self->vtable = D_08A10A10;
        func_08A100FC(self);
        func_08A081B8(&self->field_6C, 2);
        func_08A16790(self->field_3C, 2);
        func_08A017C0(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
