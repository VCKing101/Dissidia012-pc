#include "battle/auto_region3.h"

/* Part17C90 deleting destructor. */
void func_08A17540(void *object, int flags)
{
    Part17C90 *self = object;

    if (object) {
        self->vtable = D_08A17C90;
        func_08A175E8(self);
        func_08A081B8(&self->field_D8, 2);
        func_08A16D88(&self->field_50, 2);
        func_089F8248(&self->field_18, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
