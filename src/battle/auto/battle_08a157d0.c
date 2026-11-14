#include "battle/auto_region3.h"

/* Unit158B8 constructor: the base of the units. */
void *func_08A157D0(void *object)
{
    Unit158B8 *self = object;

    self->vtable = D_08A158B8;
    return self;
}
