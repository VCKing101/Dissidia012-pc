#include "battle/auto_region4.h"

/* PartB71A0 deleting destructor: releases, then destroys the base. */
void func_08AB6820(PartB71A0 *self, int flags)
{
    if (self) {
        self->vtable = D_08AB71A0;
        func_08AB68A8(self);
        func_088310BC(self, 0);
        if (flags & 1) {
            func_0883133C(self);
        }
    }
}
