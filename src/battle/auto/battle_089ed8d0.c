#include "battle/auto_region1.h"

/* PartEE950 deleting destructor. */
void func_089ED8D0(PartEE950 *self, int flags)
{
    if (self) {
        self->vtable = D_089EE950;
        func_089ED974(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
