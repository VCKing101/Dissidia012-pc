#include "menu/auto_region1.h"

/* Screen35550 deleting destructor. */
void func_08A30C64(Screen35550 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08A35550;
        func_08A408B4(self, 0);
        if ((flags & 1) != 0) {
            func_08A40920(self);
        }
    }
}
