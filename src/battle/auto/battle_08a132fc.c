#include "battle/auto_region2.h"

/* Unit14458 deleting destructor. */
void func_08A132FC(Unit14458 *self, int flags)
{
    if (self) {
        self->vtable = D_08A14458;
        func_08A13418(self);
        func_08A081B8(&self->field_1C0, 2);
        func_089F5EB0(&self->field_178, 2);
        func_08A12BF8(&self->field_110, 2);
        func_0899372C(self->field_A0, 2, sizeof(PartF8670), func_089F8248, 0, 0);
        func_08A01E08(&self->field_7C, 2);
        func_08A16790(self->field_4C, 2);
        func_08A017C0(&self->field_24, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
