#include "battle/auto_region2.h"

/* Unit0C540 constructor. */
Unit0C540 *func_08A0BBD8(Unit0C540 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A0C540;
    func_08A10AC8(&self->field_14);
    func_08A08178(&self->field_40);
    func_08A0BCA0(self);
    return self;
}
