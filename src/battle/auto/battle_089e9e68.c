#include "battle/auto_region1.h"

/* PartEA518 deleting destructor. */
void func_089E9E68(PartEA518 *self, int flags)
{
    if (self) {
        self->vtable = D_089EA518;
        func_089E9F04(self);
        func_089EA40C(self);
        func_0899372C(self->entries, 2, 0x84C, func_089EA5F0, 0, 0);
        func_0887A644(self->block, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
