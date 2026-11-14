#include "battle/auto_region4.h"

/* Part823D8 deleting destructor. */
void func_08A821A0(Part823D8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A823D8;
        func_08A82220(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
