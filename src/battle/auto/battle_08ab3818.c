#include "battle/auto_region4.h"

/* PartB3A30 start: resets it and marks it active. */
int func_08AB3818(PartB3A30 *self)
{
    func_08AB37FC(self);
    self->active = 1;
    return 0;
}
