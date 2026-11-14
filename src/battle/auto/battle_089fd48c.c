#include "battle/auto_region2.h"

/* UnitFDCA8 deleting destructor. */
void func_089FD48C(UnitFDCA8 *self, int flags)
{
    if (self) {
        self->vtable = D_089FDCA8;
        func_089FD54C(self);
        func_089FC010(&self->field_A0, 2);
        func_089FDE58(&self->field_6C, 2);
        func_08A16790(self->field_3C, 2);
        func_08A017C0(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
