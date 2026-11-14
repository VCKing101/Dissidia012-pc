#include "menu/auto_region2.h"

extern unsigned char D_08A40248[];

/* Screen40248 deleting destructor. */
void func_08A3E21C(Screen40248 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08A40248;
        func_08A408B4(self, 0);
        if ((flags & 1) != 0) {
            func_08A40920(self);
        }
    }
}
