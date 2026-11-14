#include "main/auto_region1.h"

/* Constructor of Obj9C6D60. */
Obj9C6D60 *func_08810458(Obj9C6D60 *self)
{
    self->vtable = &D_089C6D60;
    self->field_0C.vtable = &D_089C6D70;
    self->field_0C.field_00 = 0;
    self->field_0C.field_04 = 0;
    self->field_18.vtable = &D_089C6D80;
    self->field_18.field_00 = 0;
    self->field_18.field_04 = 0;
    func_088105B4(self);
    return self;
}
