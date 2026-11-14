#include "battle/auto_region4.h"

/* Part956C8 deleting destructor. */
void func_08A94E18(Part956C8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A956C8;
        func_08A94E8C(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
