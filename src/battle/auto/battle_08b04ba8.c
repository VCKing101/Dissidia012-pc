#include "battle/auto_region5.h"

/* Part05240 constructor. */
Part05240 *func_08B04BA8(Part05240 *self)
{
    self->vtable = D_08B05240;
    func_0887A61C(self->field_0C);
    func_08993560(self->field_1C, 2, sizeof(Part05B88), func_08B05260);
    func_08B04C8C(self);
    return self;
}
