#include "menu/auto_region3.h"

/* Step7D0A0: input bits 1 and 4. */
void func_08A7D16C(Step7D0A0 *self, unsigned int flags)
{
    if ((flags & 1) != 0) {
        func_08A7D1BC(self);
    }
    if ((flags & 4) != 0) {
        func_08A7D40C(self);
    }
}
