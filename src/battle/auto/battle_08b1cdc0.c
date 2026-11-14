#include "battle/auto_region5.h"

/* Part1D810 deleting destructor. */
void func_08B1CDC0(Part1D810 *self, int flags)
{
    if (self) {
        self->vtable = D_08B1D810;
        func_08B1CE74(self);
        func_08B1D77C(self);
        func_08B19108(&self->field_44, 2);
        func_0887A644(self->field_34, 2);
        func_0887A644(self->field_24, 2);
        func_0887A644(self->field_14, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
