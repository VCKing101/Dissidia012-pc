#include "battle/auto_region2.h"

/* Part02198 deleting destructor. */
void func_08A01E08(void *object, int flags)
{
    Part02198 *self = object;

    if (object) {
        self->vtable = D_08A02198;
        func_08A01E88(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
