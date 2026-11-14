#include "battle/auto_region2.h"

/* Part01DA8 deleting destructor. */
void func_08A017C0(void *object, int flags)
{
    Part01DA8 *self = object;

    if (object) {
        self->vtable = D_08A01DA8;
        func_08A01844(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
