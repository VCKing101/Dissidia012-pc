#include "battle/auto_region2.h"

/* UnitFBE98 constructor. */
UnitFBE98 *func_089FB630(UnitFBE98 *self)
{
    func_08A157D0(self);
    self->vtable = D_089FBE98;
    func_08A21478(self->field_18);
    func_089F8208(&self->field_90);
    func_08A08178(&self->field_C8);
    func_089FB70C(self);
    return self;
}
