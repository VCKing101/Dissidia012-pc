#include "battle/auto_region3.h"

/* Unit1C398 constructor. */
Unit1C398 *func_08A1B520(Unit1C398 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A1C398;
    func_08A01788(&self->field_18);
    func_08A18D48(&self->field_40);
    func_08A01DD0(&self->field_64);
    func_08A21478(&self->field_88);
    func_089F8208(&self->field_100);
    func_08A08178(&self->field_138);
    func_08A1B638(self);
    return self;
}
