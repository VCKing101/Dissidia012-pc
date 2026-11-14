#include "battle/auto_region5.h"

/* Part29ED8 constructor. */
Part29ED8 *func_08B29948(Part29ED8 *self)
{
    self->vtable = D_08B29ED8;
    func_08B2C268(&self->field_0C);
    func_08B2BB08(&self->field_34);
    func_08B29EF0(&self->field_58);
    func_08B29A14(self);
    return self;
}
