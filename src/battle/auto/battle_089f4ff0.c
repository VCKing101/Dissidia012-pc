#include "battle/auto_region1.h"

/* PartF5628 deleting destructor. */
void func_089F4FF0(PartF5628 *self, int flags)
{
    if (self) {
        self->vtable = D_089F5628;
        func_089F5098(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
