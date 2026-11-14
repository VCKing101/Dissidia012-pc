#include "battle/auto_region3.h"

/* Part21C08 constructor. */
void *func_08A21478(void *object)
{
    Part21C08 *self = object;

    self->vtable = D_08A21C08;
    func_08A21508(self);
    return self;
}
