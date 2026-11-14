#include "main/auto_region4.h"

/* Obj9CC358: constructor. */
Obj9CC358 *func_088DFCF8(Obj9CC358 *self)
{
    self->vtable = &D_089CC358;
    func_088E2A88(&self->field_5BC);
    func_088E5AC8(&self->field_D7C);
    func_088E6DC0(&self->field_E00);
    func_088E81C0(&self->field_1220);
    func_088E86F0(&self->field_1270);
    func_088EA728(&self->field_128C);
    func_088E6678(&self->field_12B0);
    func_088EA9E8(&self->field_12D8);
    func_088EA2A0(&self->field_135C);
    func_08873880(self->field_13C0);
    self->field_1680 = 0;
    self->field_17C0 = 0;
    self->field_17BA = 0;
    self->field_17BD = 0;
    func_088DFE58(self, 0);
    return self;
}
