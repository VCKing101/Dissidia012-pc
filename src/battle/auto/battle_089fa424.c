#include "battle/auto_region1.h"

/* UnitFAAC0 deleting destructor. */
void func_089FA424(UnitFAAC0 *self, int flags)
{
    if (self) {
        self->base.vtable = D_089FAAC0;
        func_089FA4C8(self);
        func_0899372C(self->field_10, 2, 0x118, func_08A17540, 0, 0);
        func_089F1304(&self->base, 0);
        if (flags & 1) {
            func_089F1354(self);
        }
    }
}
