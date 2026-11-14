#include "battle/auto_region4.h"

/* PartA5610 constructor (declared in auto_region3.h with a void *
 * parameter). */
int func_08AA4CB0(void *object)
{
    PartA5610 *self = object;

    self->vtable = D_08AA5610;
    func_08AA4D40(self);
    return (int)self;
}
