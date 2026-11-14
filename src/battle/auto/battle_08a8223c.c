#include "battle/auto_region4.h"

/* Part823D8 start: resets it and marks it active. */
int func_08A8223C(Part823D8 *self)
{
    func_08A82220(self);
    self->active = 1;
    return 0;
}
