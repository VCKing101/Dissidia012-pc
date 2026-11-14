#include "battle/auto_region3.h"

/* Unit18BD8 deleting destructor. */
void func_08A17D1C(Unit18BD8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A18BD8;
        func_08A17E04(self);
        func_08A081B8(&self->field_114, 2);
        func_08A006E0(&self->field_D8, 2);
        func_08A01E08(&self->field_B4, 2);
        func_08A16790(&self->field_84, 2);
        func_08A01E08(&self->field_60, 2);
        func_08A18D80(&self->field_3C, 2);
        func_08A017C0(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
