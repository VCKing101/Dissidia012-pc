#include "battle/auto_region5.h"

/* Part05240 deleting destructor. */
void func_08B04C00(Part05240 *self, int flags)
{
    if (self) {
        self->vtable = D_08B05240;
        func_08B04CA0(self);
        func_08B04D10(self);
        func_0899372C(self->field_1C, 2, sizeof(Part05B88), func_08B05298, 0, 0);
        func_0887A644(self->field_0C, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
