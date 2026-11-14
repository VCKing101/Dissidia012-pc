#include "battle/auto_region2.h"

/* Unit0A208 constructor. */
Unit0A208 *func_08A09898(Unit0A208 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A0A208;
    func_089F8680(&self->field_18);
    func_089F8208(&self->field_A8);
    func_08993560(self->field_E0, 2, sizeof(Part08388), func_08A08178);
    func_08A09998(self);
    return self;
}
