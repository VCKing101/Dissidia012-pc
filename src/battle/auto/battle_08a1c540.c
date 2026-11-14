#include "battle/auto_region3.h"

/* Unit1D3F8 constructor. */
Unit1D3F8 *func_08A1C540(Unit1D3F8 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A1D3F8;
    func_08A01788(&self->field_14);
    func_08A01788(&self->field_3C);
    func_08A01788(&self->field_64);
    func_089F8208(&self->field_8C);
    func_08A10AC8(&self->field_C4);
    func_08A08178(&self->field_F0);
    func_08A1C658(self);
    return self;
}
