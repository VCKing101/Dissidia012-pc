#include "battle/auto_region2.h"

/* Part13268 deleting destructor. */
void func_08A12BF8(Part13268 *self, int flags)
{
    if (self) {
        self->vtable = D_08A13268;
        func_08A12CBC(self);
        func_089F7C80(&self->field_08, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
