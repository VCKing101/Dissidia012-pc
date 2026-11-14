#include "battle/auto_region1.h"

/* UnitF97A8 deleting destructor. */
void func_089F8E7C(UnitF97A8 *self, int flags)
{
    if (self) {
        self->vtable = D_089F97A8;
        func_089F8F48(self);
        func_0899372C(self->field_E0, 2, 0x34, func_08A081B8, 0, 0);
        func_089F8248(&self->field_A8, 2);
        func_089F86B8(&self->field_18, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
