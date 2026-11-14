#include "battle/auto_region4.h"

/* Part88B28 constructor (declared in auto_region3.h with a void *
 * parameter). */
int func_08A870F0(void *object)
{
    Part88B28 *self = object;

    self->vtable = D_08A88B28;
    func_08A7E740(&self->field_2C);
    func_08A956E0(&self->field_40);
    func_08A956E0(&self->field_130);
    func_08A8A32C(&self->field_230);
    func_08A9007C(&self->field_450);
    func_08A928FC(&self->field_470);
    func_08A90ED4(&self->field_4A0);
    func_08A91ABC(&self->field_4C0);
    func_08A91E8C(&self->field_4E0);
    func_08A91574(&self->field_510);
    func_08A9239C(&self->field_530);
    func_08A8F7AC(&self->field_550);
    func_08A8F044(&self->field_590);
    func_08A906F4(&self->field_5B0);
    func_08A897DC(&self->field_600);
    func_08A89DFC(&self->field_61C);
    func_08A88B8C(&self->field_62C);
    func_08A89234(&self->field_648);
    func_08A872E8(self);
    return (int)self;
}
