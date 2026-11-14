#include "battle/auto_region4.h"

/* PartC8F00 deleting destructor. */
void func_08AC8F30(PartC8F00 *self, int flags)
{
    if (self) {
        func_08AC8FC8(self);
        func_08AC9468(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
