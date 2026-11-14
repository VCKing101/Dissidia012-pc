#include "battle/auto_region2.h"

/* Part0EB78 deleting destructor. */
void func_08A0E3D8(void *object, int flags)
{
    Part0EB78 *self = object;

    if (object) {
        self->vtable = D_08A0EB78;
        func_08A0E4A8(self);
        func_089F7C80(&self->field_08, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
