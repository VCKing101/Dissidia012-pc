#include "battle/auto_region4.h"

/* PartA29D8 deleting destructor. */
void func_08AA1F10(void *object, int flags)
{
    PartA29D8 *self = object;

    if (object) {
        self->vtable = D_08AA29D8;
        func_08AA1F8C(self);
        func_08A7E778(&self->field_14, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
