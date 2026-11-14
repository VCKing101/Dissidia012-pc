#include "menu/auto_region1.h"

/* Screen181E0 deleting destructor. */
void func_08A175A4(Screen181E0 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08A181E0;
        func_08A2405C(self, 0);
        if ((flags & 1) != 0) {
            func_08A240C8(self);
        }
    }
}
