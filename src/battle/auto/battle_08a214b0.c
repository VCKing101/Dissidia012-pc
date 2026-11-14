#include "battle/auto_region3.h"

/* Part21C08 deleting destructor. */
void func_08A214B0(void *object, int flags)
{
    Part21C08 *self = object;

    if (object) {
        self->vtable = D_08A21C08;
        func_08A21578(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
