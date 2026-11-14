#include "battle/auto_region1.h"

/* PartEBB88 deleting destructor. */
void func_089EB898(PartEBB88 *self, int flags)
{
    if (self) {
        self->vtable = D_089EBB88;
        func_089EB904(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
