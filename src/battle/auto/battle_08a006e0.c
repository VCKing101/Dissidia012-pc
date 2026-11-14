#include "battle/auto_region2.h"

/* Part00CE0 deleting destructor. */
void func_08A006E0(void *object, int flags)
{
    Part00CE0 *self = object;

    if (object) {
        self->vtable = D_08A00CE0;
        func_08A00784(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
