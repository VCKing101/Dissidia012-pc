#include "battle/auto_region3.h"

/* Unit1F628 constructor. */
Unit1F628 *func_08A1EC00(Unit1F628 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A1F628;
    func_08A10AC8(&self->field_1C);
    func_08A10AC8(&self->field_48);
    func_089F8208(&self->field_74);
    func_089F5E70(&self->field_AC);
    func_08A08178(&self->field_F4);
    func_08A1ED04(self);
    return self;
}
