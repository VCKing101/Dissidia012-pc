#include "battle/auto_region4.h"

/* PartC0FA0 deleting destructor. */
void func_08AC0B08(PartC0FA0 *self, int flags)
{
    if (self) {
        self->vtable = D_08AC0FA0;
        func_08AC0B70(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
