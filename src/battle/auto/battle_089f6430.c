#include "battle/auto_region1.h"

/* PartF65E0 deleting destructor. */
void func_089F6430(PartF65E0 *self, int flags)
{
    if (self) {
        self->vtable = D_089F65E0;
        func_089F6490(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
