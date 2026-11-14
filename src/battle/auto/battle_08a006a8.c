#include "battle/auto_region2.h"

/* Part00CE0 constructor. */
void *func_08A006A8(void *object)
{
    Part00CE0 *self = object;

    self->vtable = D_08A00CE0;
    func_08A00738(self);
    return self;
}
