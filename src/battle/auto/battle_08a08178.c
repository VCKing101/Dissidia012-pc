#include "battle/auto_region2.h"

/* Part08388 constructor. */
void *func_08A08178(void *object)
{
    Part08388 *self = object;

    self->vtable = D_08A08388;
    func_08A10AC8(&self->field_04);
    func_08A0821C(self);
    return self;
}
