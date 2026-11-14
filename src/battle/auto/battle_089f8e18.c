#include "battle/auto_region1.h"

/* UnitF97A8 constructor. */
UnitF97A8 *func_089F8E18(UnitF97A8 *self)
{
    func_08A157D0(self);
    self->vtable = D_089F97A8;
    func_089F8680(&self->field_18);
    func_089F8208(&self->field_A8);
    func_08993560(self->field_E0, 2, 0x34, func_08A08178);
    func_089F8F18(self);
    return self;
}
