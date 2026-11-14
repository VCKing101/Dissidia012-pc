#include "battle/auto_region5.h"

/* ObjFC570 deleting destructor. */
void func_08AFC514(ObjFC570 *self, int flags)
{
    if (self) {
        self->vtable = D_08AFC570;
        func_088EC234(self, 0);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
