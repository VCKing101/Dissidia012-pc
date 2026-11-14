#include "battle/auto_region2.h"

/* Unit0E2E0 constructor. */
Unit0E2E0 *func_08A0D960(Unit0E2E0 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A0E2E0;
    func_08A01788(&self->field_18);
    func_08A16758(self->field_40);
    func_08A01DD0(&self->field_70);
    func_08A0D6C0(&self->field_94);
    func_08A0DA50(self);
    return self;
}
