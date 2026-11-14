#include "battle/auto_region3.h"

/* Part16D18 deleting destructor. */
void func_08A16790(void *object, int flags)
{
    Part16D18 *self = object;

    if (object) {
        self->vtable = D_08A16D18;
        func_08A1681C(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
