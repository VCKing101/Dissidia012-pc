#include "menu/auto_region1.h"

/* Screen28560 deleting destructor. */
void func_08A2594C(Screen28560 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08A28560;
        func_08A24C80(self);
        func_08A2405C(self, 0);
        if ((flags & 1) != 0) {
            func_08A240C8(self);
        }
    }
}
