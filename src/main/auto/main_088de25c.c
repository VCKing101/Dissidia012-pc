#include "main/auto_region4.h"

/* Obj9CC290: constructor. */
Obj9CC290 *func_088DE25C(Obj9CC290 *self)
{
    self->vtable = &D_089CC290;
    func_088DF04C(&self->field_14);
    func_088DE930(&self->field_30);
    func_08993560(self->field_48, 2, sizeof(Obj9CC2D0), func_088DEC78);
    func_088DF498(&self->field_80);
    func_088DE360(self);
    return self;
}
