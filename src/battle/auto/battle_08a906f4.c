#include "battle/auto_region4.h"

/* Obj90E60 constructor. */
Obj90E60 *func_08A906F4(Obj90E60 *self)
{
    self->vtable = D_08A90E60;
    func_08A82168(&self->field_34);
    func_08A907AC(self);
    return self;
}
