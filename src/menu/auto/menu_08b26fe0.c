#include "menu/auto_region4.h"

/* Panel26C88 deleting destructor. */
void func_08B26FE0(Panel26C88 *self, unsigned int flags)
{
    if (self != 0) {
        func_08B2513C(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
