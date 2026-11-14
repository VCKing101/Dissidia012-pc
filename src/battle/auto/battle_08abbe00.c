#include "battle/auto_region4.h"

/* PartBC580 constructor: builds the Inline2 members in place. */
PartBC580 *func_08ABBE00(PartBC580 *self)
{
    self->vtable = D_08ABC580;
    self->field_08.vtable = D_08ABC590;
    self->field_08.field_00 = 0;
    self->field_08.field_04 = 0;
    self->field_14.vtable = D_08ABC5A0;
    self->field_14.field_00 = 0;
    self->field_14.field_04 = 0;
    self->field_20.vtable = D_08ABC5B0;
    self->field_20.field_00 = 0;
    self->field_20.field_04 = 0;
    self->field_2C.vtable = D_08ABC5C0;
    self->field_2C.field_00 = 0;
    self->field_2C.field_04 = 0;
    self->field_38.vtable = D_08ABC5D0;
    self->field_38.field_00 = 0;
    self->field_38.field_04 = 0;
    self->field_44.vtable = D_08ABC5E0;
    self->field_44.field_00 = 0;
    self->field_44.field_04 = 0;
    func_08ABC0D0(self);
    return self;
}
