#include "battle/auto_region4.h"

/* Part97108 constructor (declared in auto_region3.h with a void *
 * parameter). */
int func_08A96398(void *object)
{
    Part97108 *self = object;

    self->vtable = D_08A97108;
    func_08AB3758(&self->field_0C);
    self->field_70 = 1;
    func_08A96444(self);
    return (int)self;
}
