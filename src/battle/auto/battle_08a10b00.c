#include "battle/auto_region2.h"

/* Part10F98 deleting destructor. */
void func_08A10B00(void *object, int flags)
{
    Part10F98 *self = object;

    if (object) {
        self->vtable = D_08A10F98;
        func_08A10B88(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
