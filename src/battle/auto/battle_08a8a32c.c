#include "battle/auto_region4.h"

/* Obj8EFC0 constructor. */
Obj8EFC0 *func_08A8A32C(Obj8EFC0 *self)
{
    self->vtable = D_08A8EFC0;
    func_08A7E740(&self->field_3C);
    func_08AA4270(&self->field_4C);
    func_08A815D8(&self->field_74);
    func_08A98078(&self->field_88);
    func_08AA5830(&self->field_11C);
    func_08A94DE0(&self->field_164);
    func_08A82168(&self->field_180);
    func_08A94228(&self->field_190);
    func_08A85C00(&self->field_1C0);
    func_08A8A484(self);
    return self;
}
