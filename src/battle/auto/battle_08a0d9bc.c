#include "battle/auto_region2.h"

/* Unit0E2E0 deleting destructor. */
void func_08A0D9BC(Unit0E2E0 *self, int flags)
{
    if (self) {
        self->vtable = D_08A0E2E0;
        func_08A0DA84(self);
        func_08A0D6F8(&self->field_94, 2);
        func_08A01E08(&self->field_70, 2);
        func_08A16790(self->field_40, 2);
        func_08A017C0(&self->field_18, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
