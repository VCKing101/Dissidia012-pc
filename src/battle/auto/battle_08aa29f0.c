#include "battle/auto_region4.h"

/* PartA4250 constructor (declared in auto_region3.h with a void *
 * parameter). */
int func_08AA29F0(void *object)
{
    PartA4250 *self = object;

    self->vtable = D_08AA4250;
    func_08A7E740(&self->field_08);
    func_08AA2A94(self);
    return (int)self;
}
