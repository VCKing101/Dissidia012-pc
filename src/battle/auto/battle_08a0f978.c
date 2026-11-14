#include "battle/auto_region2.h"

/* Unit0FF40 constructor. */
Unit0FF40 *func_08A0F978(Unit0FF40 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A0FF40;
    func_08A0EBB0(&self->field_10);
    func_08A0FA2C(self);
    return self;
}
