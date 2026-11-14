#include "battle/auto_region3.h"

/* Part22C60 deleting destructor. */
void func_08A22AF8(Part22C60 *self, int flags)
{
    if (self) {
        self->vtable = D_08A22C60;
        func_08A22B58(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
