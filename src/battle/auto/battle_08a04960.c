#include "battle/auto_region2.h"

/* Part05658 deleting destructor. */
void func_08A04960(Part05658 *self, int flags)
{
    if (self) {
        self->vtable = D_08A05658;
        func_08A049EC(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
