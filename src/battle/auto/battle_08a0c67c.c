#include "battle/auto_region2.h"

/* Unit0CE20 deleting destructor. */
void func_08A0C67C(Unit0CE20 *self, int flags)
{
    if (self) {
        self->vtable = D_08A0CE20;
        func_08A0C730(self);
        func_08A10B00(&self->field_6C, 2);
        func_08A16790(self->field_3C, 2);
        func_08A017C0(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
