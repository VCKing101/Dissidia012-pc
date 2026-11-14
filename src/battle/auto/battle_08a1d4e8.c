#include "battle/auto_region3.h"

/* Unit1E058 constructor. */
Unit1E058 *func_08A1D4E8(Unit1E058 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A1E058;
    func_08A10AC8(&self->field_20);
    func_089F8208(&self->field_4C);
    func_089F5E70(&self->field_84);
    func_08A01788(&self->field_CC);
    func_08A16758(&self->field_F4);
    func_089FDE20(&self->field_124);
    func_089FBFD8(&self->field_158);
    func_08A1D614(self);
    return self;
}
