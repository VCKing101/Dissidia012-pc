#include "menu/auto_region1.h"

/* Elem3A4F0 deleting destructor. */
void func_08A3A5D8(Elem3A4F0 *self, unsigned int flags)
{
    if (self != 0) {
        func_08869908(self, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
