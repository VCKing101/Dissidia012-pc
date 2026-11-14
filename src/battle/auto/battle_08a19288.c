#include "battle/auto_region3.h"

/* Unit1A350 constructor. */
Unit1A350 *func_08A19288(Unit1A350 *self)
{
    func_089F12F0(&self->base);
    self->base.vtable = D_08A1A350;
    func_08993560(self->field_14, 2, sizeof(Part01DA8), func_08A01788);
    func_08993560(self->field_64, 2, sizeof(Part19278), func_08A18D48);
    func_08993560(self->field_AC, 2, sizeof(Part02198), func_08A01DD0);
    func_08993560(self->field_F4, 2, sizeof(Part17C90), func_08A174F0);
    func_08A19408(self);
    return self;
}
