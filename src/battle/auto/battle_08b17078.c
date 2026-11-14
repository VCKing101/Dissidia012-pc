#include "battle/auto_region5.h"

/* Part16D98: loads handle 0 from source 1 and starts step 1. */
void func_08B17078(Part16D98 *self)
{
    self->handles[0] = func_088DAA40(self->sources[1], 1, 0);
    self->phase = 1;
    self->step = 1;
}
