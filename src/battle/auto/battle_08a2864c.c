#include "battle/auto_region3.h"

/* Unit28BB8 deleting destructor. */
void func_08A2864C(Unit28BB8 *self, int flags)
{
    if (self) {
        self->base.vtable = D_08A28BB8;
        func_08A286E4(self);
        func_08A081B8(&self->field_14, 2);
        func_089F1304(&self->base, 0);
        if (flags & 1) {
            func_089F1354(self);
        }
    }
}
