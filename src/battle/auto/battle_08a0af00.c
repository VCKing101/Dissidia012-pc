#include "battle/auto_region2.h"

/* Unit0BAF0 constructor. */
Unit0BAF0 *func_08A0AF00(Unit0BAF0 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A0BAF0;
    func_08A10AC8(&self->field_14);
    func_08A01788(&self->field_40);
    func_08A16758(self->field_68);
    func_08A08178(&self->field_98);
    func_08A0AFF0(self);
    return self;
}
