#include "battle/auto_region1.h"

/* UnitFB568 constructor. */
UnitFB568 *func_089FAC40(UnitFB568 *self)
{
    func_08A157D0(self);
    self->vtable = D_089FB568;
    func_08A10AC8(self->field_1C);
    func_089F8208(&self->field_48);
    func_089F5E70(&self->field_80);
    func_08A08178(self->field_C8);
    func_089FAD30(self);
    return self;
}
