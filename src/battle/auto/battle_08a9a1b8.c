#include "battle/auto_region4.h"

/* Part9A528 constructor (declared in auto_region3.h with a void *
 * parameter). */
int func_08A9A1B8(void *object)
{
    Part9A528 *self = object;

    self->vtable = D_08A9A528;
    func_08A9A248(self);
    return (int)self;
}
