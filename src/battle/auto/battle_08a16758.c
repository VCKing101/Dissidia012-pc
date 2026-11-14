#include "battle/auto_region3.h"

/* Part16D18 constructor. */
void *func_08A16758(void *object)
{
    Part16D18 *self = object;

    self->vtable = D_08A16D18;
    func_08A167E8(self);
    return self;
}
