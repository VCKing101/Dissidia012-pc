#include "battle/auto_region2.h"

/* PartFE400 deleting destructor. */
void func_089FDE58(PartFE400 *self, int flags)
{
    if (self) {
        self->vtable = D_089FE400;
        func_089FDEF8(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
