#include "battle/auto_region4.h"

/* Part86C80 deleting destructor. */
void func_08A86990(Part86C80 *self, int flags)
{
    if (self) {
        self->vtable = D_08A86C80;
        func_08A86A20(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
