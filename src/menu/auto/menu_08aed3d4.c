#include "menu/auto_region3.h"

extern unsigned char D_08AEF718[];

/* Screen23F0 deleting destructor. */
void func_08AED3D4(Screen23F0 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08AEF718;
        func_08AED204(self);
        func_08AF1244(self, 0);
        if ((flags & 1) != 0) {
            func_08AF12B0(self);
        }
    }
}
