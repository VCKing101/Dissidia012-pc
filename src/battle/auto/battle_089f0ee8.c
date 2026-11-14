#include "battle/auto_region1.h"

/* PartF12D8 deleting destructor. */
void func_089F0EE8(PartF12D8 *self, int flags)
{
    if (self) {
        self->vtable = D_089F12D8;
        func_089F0F58(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
