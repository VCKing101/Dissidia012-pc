#include "battle/auto_region4.h"

/* PartBAC98 deleting destructor. */
void func_08ABA820(PartBAC98 *self, int flags)
{
    if (self) {
        self->vtable = D_08ABAC98;
        func_08ABA8AC(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
