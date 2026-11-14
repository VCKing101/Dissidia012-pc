#include "battle/auto_region4.h"

/* PartA4250 deleting destructor. */
void func_08AA2A30(void *object, int flags)
{
    PartA4250 *self = object;

    if (object) {
        self->vtable = D_08AA4250;
        func_08AA2AB8(self);
        func_08A7E778(&self->field_08, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
