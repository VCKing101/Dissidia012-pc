#include "battle/auto_region5.h"

/* Part014D8 deleting destructor. */
void func_08B0150C(Part014D8 *self, int flags)
{
    if (self) {
        func_08B015DC(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
