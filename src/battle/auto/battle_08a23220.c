#include "battle/auto_region3.h"

/* Part24BA0 deleting destructor. */
void func_08A23220(Part24BA0 *self, int flags)
{
    if (self) {
        self->vtable = D_08A24BA0;
        func_08A232D4(self);
        func_08A23930(self);
        func_089EEBC0(&self->field_80, 2);
        func_089EEBC0(&self->field_10, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
