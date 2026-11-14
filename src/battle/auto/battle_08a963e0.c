#include "battle/auto_region4.h"

/* Part97108 deleting destructor. */
void func_08A963E0(void *object, int flags)
{
    Part97108 *self = object;

    if (object) {
        self->vtable = D_08A97108;
        func_08A9649C(self);
        func_08AB3790(&self->field_0C, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
