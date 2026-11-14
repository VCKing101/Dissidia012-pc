#include "battle/auto_region1.h"

/* PartF3820 deleting destructor. */
void func_089F3458(PartF3820 *self, int flags)
{
    if (self) {
        self->vtable = D_089F3820;
        func_089F34D4(self);
        func_089F7C80(&self->field_08, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
