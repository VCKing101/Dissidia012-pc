#include "battle/auto_region3.h"

/* Unit28520 constructor. */
Unit28520 *func_08A27A90(Unit28520 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A28520;
    func_08A01788(&self->field_1C);
    func_08A16758(&self->field_44);
    func_089FDE20(&self->field_74);
    func_089FC668(&self->field_A8);
    func_08A27B80(self);
    return self;
}
