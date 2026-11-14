#include "battle/auto_region1.h"

/* PartF63E0 deleting destructor. */
void func_089F5EB0(PartF63E0 *self, int flags)
{
    if (self) {
        self->vtable = D_089F63E0;
        func_089F5F44(self);
        func_089F7C80(&self->field_10, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
