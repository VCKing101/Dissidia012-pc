#include "battle/auto_region3.h"

/* Part157B8 deleting destructor. */
void func_08A15578(void *object, int flags)
{
    Part157B8 *self = object;

    if (object) {
        self->vtable = D_08A157B8;
        func_08A155DC(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
