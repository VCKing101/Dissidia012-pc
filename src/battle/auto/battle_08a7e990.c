#include "battle/auto_region3.h"

/* Part7FF38 deleting destructor. */
void func_08A7E990(Part7FF38 *self, int flags)
{
    if (self) {
        self->vtable = D_08A7FF38;
        func_08A7EA74(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
