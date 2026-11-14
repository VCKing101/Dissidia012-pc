#include "battle/auto_region3.h"

/* Releases the part, then marks it active. */
int func_08A15A40(Part15B58 *self)
{
    func_08A15A04(self);
    self->active = 1;
    return 0;
}
