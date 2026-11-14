#include "battle/auto_region4.h"

/* Part94DB8 deleting destructor. */
void func_08A94260(void *object, int flags)
{
    Part94DB8 *self = object;

    if (object) {
        self->vtable = D_08A94DB8;
        func_08A94300(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
