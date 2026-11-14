#include "battle/auto_region1.h"

/* PartF8670 deleting destructor. */
void func_089F8248(PartF8670 *self, int flags)
{
    if (self) {
        self->vtable = D_089F8670;
        func_089F82C8(self);
        func_089F7C80(&self->field_14, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
