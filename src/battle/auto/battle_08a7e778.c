#include "battle/auto_region3.h"

/* Part7E948 deleting destructor. */
void func_08A7E778(Part7E948 *self, int flags)
{
    if (self) {
        self->vtable = D_08A7E948;
        func_08A7E7E0(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
