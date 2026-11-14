#include "battle/auto_region4.h"

/* PartCEE28 start: resets it and marks it active. */
int func_08ACEEC4(PartCEE28 *self)
{
    func_08ACEEA8(self);
    self->active = 1;
    return 0;
}
