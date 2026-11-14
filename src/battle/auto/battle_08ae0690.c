#include "battle/auto_region4.h"

/* PartE7B50 deleting destructor. */
void func_08AE0690(PartE7B50 *self, int flags)
{
    if (self) {
        self->vtable = D_08AE7B50;
        func_08AE08DC(self);
        if (flags & 1) {
            func_08AFCB44(self);
        }
    }
}
