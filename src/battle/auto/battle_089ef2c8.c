#include "battle/auto_region1.h"

/* PartEFC58 deleting destructor. */
void func_089EF2C8(PartEFC58 *self, int flags)
{
    if (self) {
        self->vtable = D_089EFC58;
        func_089EF350(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
