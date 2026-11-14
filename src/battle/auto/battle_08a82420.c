#include "battle/auto_region4.h"

/* Part83A38 deleting destructor. */
void func_08A82420(Part83A38 *self, int flags)
{
    if (self) {
        self->vtable = D_08A83A38;
        func_08A824BC(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
