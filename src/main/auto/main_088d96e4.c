#include "main/auto_region4.h"

/* Obj9CC250: deleting destructor. */
void func_088D96E4(Obj9CC250 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089CC250;
        func_088D9750(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
