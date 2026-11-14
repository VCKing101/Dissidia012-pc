#include "menu/auto_region2.h"

extern unsigned char D_08A47D90[];

/* Screen47D90 deleting destructor. */
void func_08A47234(Screen47D90 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08A47D90;
        func_08A408B4(self, 0);
        if ((flags & 1) != 0) {
            func_08A40920(self);
        }
    }
}
