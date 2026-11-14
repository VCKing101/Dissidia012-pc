#include "battle/auto_region2.h"

/* UnitFB568 deleting destructor. */
void func_089FAC9C(UnitFB568 *self, int flags)
{
    if (self) {
        self->vtable = D_089FB568;
        func_089FAD64(self);
        func_08A081B8(self->field_C8, 2);
        func_089F5EB0(&self->field_80, 2);
        func_089F8248(&self->field_48, 2);
        func_08A10B00(self->field_1C, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
