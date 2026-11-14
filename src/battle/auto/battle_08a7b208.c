#include "battle/auto_region3.h"

/* Part7E728 deleting destructor. */
void func_08A7B208(Part7E728 *self, int flags)
{
    if (self) {
        self->vtable = D_08A7E728;
        func_08A7B288(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
