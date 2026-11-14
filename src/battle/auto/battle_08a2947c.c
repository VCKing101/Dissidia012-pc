#include "battle/auto_region3.h"

/* Unit29D08 deleting destructor. */
void func_08A2947C(Unit29D08 *self, int flags)
{
    if (self) {
        self->vtable = D_08A29D08;
        func_08A29544(self);
        func_089F5EB0(&self->field_AC, 2);
        func_089F8248(&self->field_74, 2);
        func_08A16790(&self->field_44, 2);
        func_08A017C0(&self->field_1C, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
