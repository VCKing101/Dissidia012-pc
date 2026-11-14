#include "battle/auto_region3.h"

/* Releases the part, then marks it active. */
int func_08A65794(Part65E48 *self)
{
    func_08A65778(self);
    self->active = 1;
    return 0;
}
