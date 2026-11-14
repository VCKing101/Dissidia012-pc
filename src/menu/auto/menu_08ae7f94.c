#include "menu/auto_region3.h"

extern unsigned char D_08AEB118[];

/* ScreenA690 deleting destructor. */
void func_08AE7F94(ScreenA690 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08AEB118;
        func_08AE797C(self);
        func_08AF1244(self, 0);
        if ((flags & 1) != 0) {
            func_08AF12B0(self);
        }
    }
}
