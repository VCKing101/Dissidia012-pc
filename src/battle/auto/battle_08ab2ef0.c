#include "battle/auto_region4.h"

/* PartB32A8 constructor (declared in auto_region3.h with a void *
 * parameter). */
int func_08AB2EF0(void *object)
{
    PartB32A8 *self = object;

    self->vtable = D_08AB32A8;
    func_08A97118(&self->field_04);
    func_08A823E8(&self->field_198);
    func_08AAE188(&self->field_34C);
    func_08A64EA8(&self->field_360);
    func_08A7B1D0(&self->field_36C);
    func_08A7E958(&self->field_390);
    func_08A86C90(&self->field_440);
    func_08AB300C(self);
    return (int)self;
}
