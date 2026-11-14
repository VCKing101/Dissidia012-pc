#include "battle/auto_region4.h"

/* Part9ABE8 deleting destructor. */
void func_08A9A578(void *object, int flags)
{
    Part9ABE8 *self = object;

    if (object) {
        self->vtable = D_08A9ABE8;
        func_08A9A5EC(self);
        func_08A7E778(&self->field_0C, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
