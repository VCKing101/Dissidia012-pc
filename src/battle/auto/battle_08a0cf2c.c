#include "battle/auto_region2.h"

/* Unit0D608 deleting destructor. */
void func_08A0CF2C(Unit0D608 *self, int flags)
{
    if (self) {
        self->vtable = D_08A0D608;
        func_08A0CFD4(self);
        func_08A16790(self->field_3C, 2);
        func_08A017C0(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
