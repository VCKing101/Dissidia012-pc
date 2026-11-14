#include "battle/auto_region1.h"

/* PartF81A8 deleting destructor. */
void func_089F7C80(PartF81A8 *self, int flags)
{
    if (self) {
        self->vtable = D_089F81A8;
        func_089F7D0C(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
