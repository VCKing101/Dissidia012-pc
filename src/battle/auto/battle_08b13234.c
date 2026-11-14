#include "battle/auto_region5.h"

/* Part13234 constructor: both delegates forward to this object. */
Part13234 *func_08B13234(Part13234 *self)
{
    self->field_04.vtable = D_08B13AF8;
    self->field_04.owner = self;
    self->field_0C.vtable = D_08B13B08;
    self->field_0C.owner = self;
    self->field_14 = 0;
    func_08993560(self->entries, 8, 0x78, func_08B131F8);
    self->field_3DC = 0;
    self->field_3E0 = 0;
    self->field_3D8 = 0;
    return self;
}
