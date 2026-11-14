#include "battle/auto_region5.h"

/* Part16D98: in step 4, plays motion 2 on handle 2 and finishes. */
void func_08B17270(Part16D98 *self)
{
    if (self->phase == 1 && self->step == 4) {
        func_088D9F5C(self->handles[2], 2, 1);
        self->phase = 2;
    }
}
