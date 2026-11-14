#include "main/auto_region2.h"

/* Deleting destructor of an array of forty-five 0x68-byte elements. */
void func_08879EC4(Obj879EC4 *self, unsigned int flags)
{
    if (self != 0) {
        func_0899372C(self, 0x2d, 0x68, func_08879E60, 0, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
