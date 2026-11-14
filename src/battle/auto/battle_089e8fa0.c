#include "battle/auto_region1.h"

/* PartE9568 deleting destructor. */
void func_089E8FA0(PartE9568 *self, int flags)
{
    if (self) {
        self->vtable = D_089E9568;
        func_089E9024(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
