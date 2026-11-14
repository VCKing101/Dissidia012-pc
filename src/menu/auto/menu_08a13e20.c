#include "menu/auto_region1.h"

/* Screen15A88 deleting destructor. */
void func_08A13E20(Screen15A88 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08A15A88;
        func_08A2405C(self, 0);
        if ((flags & 1) != 0) {
            func_08A240C8(self);
        }
    }
}
