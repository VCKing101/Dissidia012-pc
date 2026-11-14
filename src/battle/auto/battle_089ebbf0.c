#include "battle/auto_region1.h"

/* PartECED0 deleting destructor. */
void func_089EBBF0(PartECED0 *self, int flags)
{
    if (self) {
        self->vtable = D_089ECED0;
        func_089EBCC8(self);
        func_089EB7D0(&self->field_44, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
