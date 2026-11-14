#include "battle/auto_region1.h"

/* PartF3E68 deleting destructor. */
void func_089F3890(PartF3E68 *self, int flags)
{
    if (self) {
        self->vtable = D_089F3E68;
        func_089F3908(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
