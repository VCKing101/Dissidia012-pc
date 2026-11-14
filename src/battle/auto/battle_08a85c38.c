#include "battle/auto_region4.h"

/* Part86940 deleting destructor. */
void func_08A85C38(Part86940 *self, int flags)
{
    if (self) {
        self->vtable = D_08A86940;
        func_08A85D14(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
