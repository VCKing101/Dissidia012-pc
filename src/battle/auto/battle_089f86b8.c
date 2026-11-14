#include "battle/auto_region1.h"

/* PartF8E08 deleting destructor. */
void func_089F86B8(PartF8E08 *self, int flags)
{
    if (self) {
        self->vtable = D_089F8E08;
        func_089F8788(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
