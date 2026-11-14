#include "battle/auto_region3.h"

/* Unit2A4B8 deleting destructor. */
void func_08A29E14(Unit2A4B8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A2A4B8;
        func_08A29EBC(self);
        func_08A16790(&self->field_3C, 2);
        func_08A017C0(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
