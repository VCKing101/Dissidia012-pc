#include "battle/auto_region4.h"

/* PartA1BD8 deleting destructor. */
void func_08A9AC58(void *object, int flags)
{
    PartA1BD8 *self = object;

    if (object) {
        self->vtable = D_08AA1BD8;
        func_08A9ADA8(self);
        func_08A7E778(&self->field_148, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
