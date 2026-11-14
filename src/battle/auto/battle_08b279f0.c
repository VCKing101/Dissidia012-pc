#include "battle/auto_region5.h"

/* Part28570 deleting destructor. */
void func_08B279F0(Part28570 *self, int flags)
{
    if (self) {
        self->vtable = D_08B28570;
        func_08B27A9C(self);
        func_08B27AE0(self);
        func_08B285C8(&self->field_2B0, 2);
        func_08B28EC0(&self->field_248, 2);
        func_08B29998(&self->field_24, 2);
        func_0887A644(self->field_14, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
