#include "battle/auto_region2.h"

/* Part08880 deleting destructor. */
void func_08A083F8(Part08880 *self, int flags)
{
    if (self) {
        self->vtable = D_08A08880;
        func_08A08480(self);
        func_089F7C80(&self->field_08, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
