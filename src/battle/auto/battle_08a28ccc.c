#include "battle/auto_region3.h"

/* Unit29370 deleting destructor. */
void func_08A28CCC(Unit29370 *self, int flags)
{
    if (self) {
        self->base.vtable = D_08A29370;
        func_08A28D7C(self);
        func_08A255F8(&self->field_248, 2);
        func_0899372C(self->field_10, 2, sizeof(Part268C8), func_08A26218, 0, 0);
        func_089F1304(&self->base, 0);
        if (flags & 1) {
            func_089F1354(self);
        }
    }
}
