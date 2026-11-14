#include "battle/auto_region2.h"

/* Part048C8 deleting destructor. */
void func_08A038B8(Part048C8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A048C8;
        func_08A03944(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
