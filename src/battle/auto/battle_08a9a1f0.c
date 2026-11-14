#include "battle/auto_region4.h"

/* Part9A528 deleting destructor. */
void func_08A9A1F0(void *object, int flags)
{
    Part9A528 *self = object;

    if (object) {
        self->vtable = D_08A9A528;
        func_08A9A25C(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
