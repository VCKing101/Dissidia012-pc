#include "menu/auto_region1.h"

/* Deleting destructor of the class with vtable D_089EC280. */
void func_089EBB30(MenuObject *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_089EC280;
        func_089EACFC(self, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
