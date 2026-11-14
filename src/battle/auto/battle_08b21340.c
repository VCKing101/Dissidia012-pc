#include "battle/auto_region5.h"

/* Part22EF8 deleting destructor. */
void func_08B21340(Part22EF8 *self, int flags)
{
    if (self) {
        self->vtable = D_08B22EF8;
        func_08B21778(self);
        func_0899372C(self->field_14, 2, sizeof(Part05658), func_08A04960, 0, 0);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
