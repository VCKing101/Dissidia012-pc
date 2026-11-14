#include "battle/auto_region1.h"

/* UnitF1B20 deleting destructor. */
void func_089F14CC(UnitF1B20 *self, int flags)
{
    if (self) {
        self->base.vtable = D_089F1B20;
        func_089F1564(self);
        func_089F1C10(&self->field_14, 2);
        func_089F1304(&self->base, 0);
        if (flags & 1) {
            func_089F1354(self);
        }
    }
}
