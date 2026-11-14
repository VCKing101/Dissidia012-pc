#include "battle/auto_region2.h"

/* Part08388 deleting destructor. */
void func_08A081B8(void *object, int flags)
{
    Part08388 *self = object;

    if (object) {
        self->vtable = D_08A08388;
        func_08A08224(self);
        func_08A10B00(&self->field_04, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
