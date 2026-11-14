#include "battle/auto_region2.h"

/* Part11640 deleting destructor. */
void func_08A112E0(Part11640 *self, int flags)
{
    if (self) {
        self->vtable = D_08A11640;
        func_08A11350(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
