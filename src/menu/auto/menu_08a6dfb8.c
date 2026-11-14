#include "menu/auto_region2.h"

extern unsigned char D_08A6E018[];

/* State D_08A6E018 deleting destructor. */
void func_08A6DFB8(State6D7E8 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08A6E018;
        func_08A6D7E8(self, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
