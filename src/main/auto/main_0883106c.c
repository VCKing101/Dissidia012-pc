#include "main/auto_region2.h"

/* Deleting destructor over the base destructor func_088BF17C. */
void func_0883106C(void *self, unsigned int flags)
{
    if (self != 0) {
        func_088BF17C(self, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
