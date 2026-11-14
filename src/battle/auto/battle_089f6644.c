#include "battle/auto_region1.h"

/* UnitF7040 deleting destructor. */
void func_089F6644(UnitF7040 *self, int flags)
{
    if (self) {
        self->vtable = D_089F7040;
        func_089F6700(self);
        func_089F8248(&self->field_7C, 2);
        func_08A081B8(self->field_48, 2);
        func_08A10B00(self->field_1C, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
