#include "battle/auto_region3.h"

/* Part65E48 deleting destructor. */
void func_08A65710(Part65E48 *self, int flags)
{
    if (self) {
        self->vtable = D_08A65E48;
        func_08A65778(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
