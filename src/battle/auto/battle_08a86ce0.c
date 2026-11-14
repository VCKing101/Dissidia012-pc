#include "battle/auto_region4.h"

/* Part870D8 deleting destructor: destroys the three Part86C80 at field_20. */
void func_08A86CE0(Part870D8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A870D8;
        func_08A86D94(self);
        func_0899372C(self->field_20, 3, sizeof(Part86C80), func_08A86990, 0, 0);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
