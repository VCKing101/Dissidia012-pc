#include "menu/auto_region4.h"

extern unsigned char D_08B349B8[];

/* Obj344E0 deleting destructor. */
void func_08B344E0(Obj344E0 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08B349B8;
        func_088310BC(self, 0);
        if ((flags & 1) != 0) {
            func_0883133C(self);
        }
    }
}
