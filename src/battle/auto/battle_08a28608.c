#include "battle/auto_region3.h"

/* Unit28BB8 constructor. */
Unit28BB8 *func_08A28608(Unit28BB8 *self)
{
    func_089F12F0(&self->base);
    self->base.vtable = D_08A28BB8;
    func_08A08178(&self->field_14);
    func_08A286BC(self);
    return self;
}
