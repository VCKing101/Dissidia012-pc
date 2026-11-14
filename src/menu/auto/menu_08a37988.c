#include "menu/auto_region1.h"

/* Screen38EA8 deleting destructor. */
void func_08A37988(Screen38EA8 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08A38EA8;
        func_08A408B4(self, 0);
        if ((flags & 1) != 0) {
            func_08A40920(self);
        }
    }
}
