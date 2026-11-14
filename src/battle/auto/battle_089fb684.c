#include "battle/auto_region2.h"

/* UnitFBE98 deleting destructor. */
void func_089FB684(UnitFBE98 *self, int flags)
{
    if (self) {
        self->vtable = D_089FBE98;
        func_089FB73C(self);
        func_08A081B8(&self->field_C8, 2);
        func_089F8248(&self->field_90, 2);
        func_08A214B0(self->field_18, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
