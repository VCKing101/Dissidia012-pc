#include "battle/auto_region4.h"

/* PartA29D8 constructor (declared in auto_region3.h with a void *
 * parameter). */
int func_08AA1ED0(void *object)
{
    PartA29D8 *self = object;

    self->vtable = D_08AA29D8;
    func_08A7E740(&self->field_14);
    func_08AA1F74(self);
    return (int)self;
}
