#include "main/auto_region2.h"

/* Deleting destructor of Pool86CE7C: destroys the items and the elements. */
void func_088799F0(Pool86CE7C *self, unsigned int flags)
{
    if (self != 0) {
        func_0899372C(self->items, 0x180, 0x1c, func_08865488, 0, 0);
        func_0899372C(self->elems, 0xc0, 0x13c, func_08869C74, 0, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
