#include "battle/auto_region4.h"

/* PartA1BD8 constructor (declared in auto_region3.h with a void *
 * parameter). */
int func_08A9AC18(void *object)
{
    PartA1BD8 *self = object;

    self->vtable = D_08AA1BD8;
    func_08A7E740(&self->field_148);
    func_08A9ACBC(self);
    return (int)self;
}
