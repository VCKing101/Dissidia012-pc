#include "battle/auto_region3.h"

/* Part7ABD0 constructor. */
Part7ABD0 *func_08A65E58(Part7ABD0 *self)
{
    self->vtable = D_08A7ABD0;
    func_08A7E740(&self->field_04);
    func_08A7E740(&self->field_14);
    func_08A94228(self->field_24);
    func_08A65F24(self);
    return self;
}
