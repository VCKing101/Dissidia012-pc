#include "menu/auto_region1.h"

/* StepCounter deleting destructor. */
void func_089EB628(StepCounter *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_089EC230;
        func_089EACFC(self, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
