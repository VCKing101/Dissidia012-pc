#include "battle/auto_region4.h"

/* PartA5830 deleting destructor. */
void func_08AA5858(PartA5830 *self, int flags)
{
    if (self) {
        func_08AA5914(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
