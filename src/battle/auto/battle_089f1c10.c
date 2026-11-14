#include "battle/auto_region1.h"

/* PartF1FE0 deleting destructor. */
void func_089F1C10(PartF1FE0 *self, int flags)
{
    if (self) {
        self->vtable = D_089F1FE0;
        func_089F1C80(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
