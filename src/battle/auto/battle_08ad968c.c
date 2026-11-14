#include "battle/auto_region4.h"

/* PartDB2A0 deleting destructor: releases, then destroys the base. */
void func_08AD968C(PartDB2A0 *self, int flags)
{
    if (self) {
        self->vtable = D_08ADB2A0;
        func_08ADA06C(self);
        func_08AD2A10(self, 0);
        if (flags & 1) {
            func_08AD2F38(self);
        }
    }
}
