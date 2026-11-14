#include "battle/auto_region3.h"

/* Part65650 deleting destructor. */
void func_08A64EE0(Part65650 *self, int flags)
{
    if (self) {
        self->vtable = D_08A65650;
        func_08A64F44(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
