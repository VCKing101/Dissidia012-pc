#include "battle/auto_region5.h"

/* Part16D98: loads handle 0 from source 2 and starts step 2. */
void func_08B170C0(Part16D98 *self)
{
    self->handles[0] = func_088DAA40(self->sources[2], 0, 0);
    self->phase = 1;
    self->step = 2;
}
