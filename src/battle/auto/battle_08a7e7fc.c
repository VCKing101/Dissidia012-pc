#include "battle/auto_region3.h"

/* Releases the part, then marks it active. */
int func_08A7E7FC(Part7E948 *self)
{
    func_08A7E7E0(self);
    self->active = 1;
    return 0;
}
