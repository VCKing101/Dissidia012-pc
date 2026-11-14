#include "menu/auto_region1.h"

/* ObjC1E0 deleting destructor. */
void func_089EAF44(ObjC1E0 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_089EC1E0;
        func_089EACFC(self, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
