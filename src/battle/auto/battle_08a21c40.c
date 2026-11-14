#include "battle/auto_region3.h"

/* Part21F38 constructor. */
void *func_08A21C40(void *object)
{
    Part21F38 *self = object;

    self->vtable = D_08A21F38;
    func_08A21CD0(self);
    return self;
}
