#include "battle/auto_region3.h"

/* Releases the part, then marks it active. */
int func_08A1561C(Part157B8 *self)
{
    func_08A155DC(self);
    self->active = 1;
    return 0;
}
