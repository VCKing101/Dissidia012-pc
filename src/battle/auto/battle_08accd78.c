#include "battle/auto_region4.h"

/* PartCD0C8 constructor. */
PartCD0C8 *func_08ACCD78(PartCD0C8 *self)
{
    self->vtable = D_08ACD0C8;
    self->field_0C = 0;
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_18 = 0;
    func_08ACCE58(self);
    return self;
}
