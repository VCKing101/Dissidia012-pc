#include "battle/auto_region1.h"

/* PartE8D08 deleting destructor. */
void func_089E8270(PartE8D08 *self, int flags)
{
    if (self) {
        self->vtable = D_089E8D08;
        func_089E831C(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
