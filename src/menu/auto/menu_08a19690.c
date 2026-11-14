#include "menu/auto_region1.h"

/* Screen1AB08 deleting destructor. */
void func_08A19690(Screen1AB08 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08A1AB08;
        func_08A2405C(self, 0);
        if ((flags & 1) != 0) {
            func_08A240C8(self);
        }
    }
}
