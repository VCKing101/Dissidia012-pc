#include "battle/auto_region1.h"

/* UnitF4ED0 constructor. */
UnitF4ED0 *func_089F3E88(UnitF4ED0 *self)
{
    func_08A157D0(self);
    self->vtable = D_089F4ED0;
    func_089F3858(&self->field_20);
    func_08A10AC8(self->field_3C);
    func_08993560(self->field_68, 2, sizeof(PartF8670), func_089F8208);
    func_08993560(self->field_E0, 2, sizeof(PartF3820), func_089F3418);
    func_08993560(self->field_360, 2, 0x34, func_08A08178);
    func_089F3FF8(self);
    return self;
}
