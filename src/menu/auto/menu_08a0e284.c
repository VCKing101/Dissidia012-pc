#include "menu/auto_region1.h"

/* Screen11510 deleting destructor. */
void func_08A0E284(Screen11510 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08A11510;
        func_08A2405C(self, 0);
        if ((flags & 1) != 0) {
            func_08A240C8(self);
        }
    }
}
