#include "battle/auto_region3.h"

/* Part15B58 deleting destructor. */
void func_08A159A0(void *object, int flags)
{
    Part15B58 *self = object;

    if (object) {
        self->vtable = D_08A15B58;
        func_08A15A04(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
