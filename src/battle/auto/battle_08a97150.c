#include "battle/auto_region4.h"

/* Part98058 deleting destructor. */
void func_08A97150(Part98058 *self, int flags)
{
    if (self) {
        self->vtable = D_08A98058;
        func_08A97240(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
