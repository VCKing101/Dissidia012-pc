#include "battle/auto_region3.h"

/* Unit165C8 constructor. */
Unit165C8 *func_08A15B68(Unit165C8 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A165C8;
    func_08A10AC8(&self->field_18);
    func_089F8208(&self->field_44);
    func_08A0E398(&self->field_7C);
    func_08A08178(&self->field_114);
    func_08A15C58(self);
    return self;
}
