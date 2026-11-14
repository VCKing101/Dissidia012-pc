#include "battle/auto_region5.h"

/* Part16D98: loads handle 1 from source 3 and starts step 3. */
void func_08B17104(Part16D98 *self)
{
    self->handles[1] = func_088DAA40(self->sources[3], 1, 0);
    self->phase = 1;
    self->step = 3;
}
