#include "battle/auto_region4.h"

/* Part94DB8 constructor (declared in auto_region3.h with a void *
 * parameter). */
int func_08A94228(void *object)
{
    Part94DB8 *self = object;

    self->vtable = D_08A94DB8;
    func_08A942B8(self);
    return (int)self;
}
