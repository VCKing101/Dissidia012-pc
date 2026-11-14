#include "battle/auto_region1.h"

/* UnitF4ED0 deleting destructor. */
void func_089F3F1C(UnitF4ED0 *self, int flags)
{
    if (self) {
        self->vtable = D_089F4ED0;
        func_089F404C(self);
        func_0899372C(self->field_360, 2, 0x34, func_08A081B8, 0, 0);
        func_0899372C(self->field_E0, 2, sizeof(PartF3820), func_089F3458, 0, 0);
        func_0899372C(self->field_68, 2, sizeof(PartF8670), func_089F8248, 0, 0);
        func_08A10B00(self->field_3C, 2);
        func_089F3890(&self->field_20, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
