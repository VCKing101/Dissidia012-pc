#include "battle/auto_region3.h"

/* Part15B58 constructor. */
void *func_08A15968(void *object)
{
    Part15B58 *self = object;

    self->vtable = D_08A15B58;
    func_08A159F8(self);
    return self;
}
