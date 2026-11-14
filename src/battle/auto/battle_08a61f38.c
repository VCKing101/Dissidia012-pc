#include "battle/auto_region3.h"

/* Part61F84 deleting destructor. */
void func_08A61F38(Part61F84 *self, int flags)
{
    if (self) {
        func_08A61FAC(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
