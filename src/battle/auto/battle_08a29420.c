#include "battle/auto_region3.h"

/* Unit29D08 constructor. */
Unit29D08 *func_08A29420(Unit29D08 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A29D08;
    func_08A01788(&self->field_1C);
    func_08A16758(&self->field_44);
    func_089F8208(&self->field_74);
    func_089F5E70(&self->field_AC);
    func_08A29510(self);
    return self;
}
