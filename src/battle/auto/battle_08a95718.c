#include "battle/auto_region4.h"

/* Part96380 deleting destructor. */
void func_08A95718(Part96380 *self, int flags)
{
    if (self) {
        self->vtable = D_08A96380;
        func_08A95874(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
