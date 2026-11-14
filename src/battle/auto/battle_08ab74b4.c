#include "battle/auto_region4.h"

/* PartB7560 start: resets it and marks it active. */
int func_08AB74B4(PartB7560 *self)
{
    func_08AB7498(self);
    self->active = 1;
    return 0;
}
