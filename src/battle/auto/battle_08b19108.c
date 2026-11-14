#include "battle/auto_region5.h"

/* Part19300 deleting destructor. */
void func_08B19108(Part19300 *self, int flags)
{
    if (self) {
        self->vtable = D_08B19300;
        func_08B19178(self);
        func_08B19358(&self->field_04, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
