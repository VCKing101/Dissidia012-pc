#include "battle/auto_region2.h"

/* Unit0AE30 constructor. */
Unit0AE30 *func_08A0A3A8(Unit0AE30 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A0AE30;
    func_08A10AC8(&self->field_14);
    func_089F63F0(&self->field_40);
    func_08A08178(&self->field_978);
    func_08A0A484(self);
    return self;
}
