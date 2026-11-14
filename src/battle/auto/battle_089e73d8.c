#include "battle/auto_region1.h"

/* PartE7988 deleting destructor. */
void func_089E73D8(PartE7988 *self, int flags)
{
    if (self) {
        self->vtable = D_089E7988;
        func_089E7450(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
