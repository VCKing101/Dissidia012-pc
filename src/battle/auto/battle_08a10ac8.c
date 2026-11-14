#include "battle/auto_region2.h"

/* Part10F98 constructor. */
void *func_08A10AC8(void *object)
{
    Part10F98 *self = object;

    self->vtable = D_08A10F98;
    func_08A10B58(self);
    return self;
}
