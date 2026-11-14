#include "battle/auto_region5.h"

/* Part29ED8 deleting destructor. */
void func_08B29998(Part29ED8 *self, int flags)
{
    if (self) {
        self->vtable = D_08B29ED8;
        func_08B29A30(self);
        func_08B29F28(&self->field_58, 2);
        func_08B2BB40(&self->field_34, 2);
        func_08B2C2A0(&self->field_0C, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
