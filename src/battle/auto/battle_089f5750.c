#include "battle/auto_region1.h"

/* PartF5E58 deleting destructor. */
void func_089F5750(PartF5E58 *self, int flags)
{
    if (self) {
        self->vtable = D_089F5E58;
        func_089F5808(self);
        func_0899372C(self->field_04, 3, sizeof(PartF5628), func_089F4FF0, 0, 0);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
