#include "battle/auto_region3.h"

/* Unit28520 deleting destructor. */
void func_08A27AEC(Unit28520 *self, int flags)
{
    if (self) {
        self->vtable = D_08A28520;
        func_08A27BB4(self);
        func_089FC6A0(&self->field_A8, 2);
        func_089FDE58(&self->field_74, 2);
        func_08A16790(&self->field_44, 2);
        func_08A017C0(&self->field_1C, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
