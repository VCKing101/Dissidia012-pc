#include "battle/auto_region4.h"

/* PartC03B8 deleting destructor. */
void func_08ABDEBC(PartC03B8 *self, int flags)
{
    if (self) {
        self->vtable = D_08AC03B8;
        func_08ABDF94(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
