#include "battle/auto_region4.h"

/* Part9ABE8 constructor (declared in auto_region3.h with a void *
 * parameter). */
int func_08A9A538(void *object)
{
    Part9ABE8 *self = object;

    self->vtable = D_08A9ABE8;
    func_08A7E740(&self->field_0C);
    func_08A9A5DC(self);
    return (int)self;
}
