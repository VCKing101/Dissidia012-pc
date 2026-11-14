#include "menu/auto_region3.h"

extern unsigned char D_08AE4550[];

/* Screen850 deleting destructor. */
void func_08AE29B0(Screen850 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08AE4550;
        func_08AF1244(self, 0);
        if ((flags & 1) != 0) {
            func_08AF12B0(self);
        }
    }
}
