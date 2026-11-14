#include "battle/auto_region5.h"

/* Part1D810 constructor. */
Part1D810 *func_08B1CD68(Part1D810 *self)
{
    self->vtable = D_08B1D810;
    func_0887A61C(self->field_14);
    func_0887A61C(self->field_24);
    func_0887A61C(self->field_34);
    func_08B190C8(&self->field_44);
    func_08B1CE50(self);
    return self;
}
