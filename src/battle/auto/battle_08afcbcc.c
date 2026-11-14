#include "battle/auto_region5.h"

/* Deleting destructor: runs the (empty) release, then frees the object. */
void func_08AFCBCC(void *self, int flags)
{
    if (self) {
        func_08AFCC78(self);
        if (flags & 1) {
            func_08AFCD90(self);
        }
    }
}
