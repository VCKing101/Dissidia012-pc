#include "battle/auto_region3.h"

/* Part254D0 deleting destructor. */
void func_08A24C80(Part254D0 *self, int flags)
{
    if (self) {
        self->vtable = D_08A254D0;
        func_08A24D40(self);
        func_089F7C80(&self->field_08, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
