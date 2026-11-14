#include "battle/auto_region2.h"

/* Part0F8D8 deleting destructor. */
void func_08A0EC18(Part0F8D8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A0F8D8;
        func_08A0ED3C(self);
        func_0899372C(self->field_A4, 2, sizeof(Part08388), func_08A081B8, 0, 0);
        func_0899372C(self->field_34, 2, sizeof(PartF8670), func_089F8248, 0, 0);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
