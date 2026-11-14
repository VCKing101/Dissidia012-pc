#include "battle/auto_region4.h"

/* PartB8390 deleting destructor. */
void func_08AB83B8(PartB8390 *self, int flags)
{
    if (self) {
        func_08AB843C(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
