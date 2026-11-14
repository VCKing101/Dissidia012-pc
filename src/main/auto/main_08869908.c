#include "main/auto_region2.h"

/* Deleting destructor over func_088699F4. */
void func_08869908(void *self, unsigned int flags)
{
    if (self != 0) {
        func_088699F4(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
