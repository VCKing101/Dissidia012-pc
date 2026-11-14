#include "main/auto_region1.h"

/* Clears bit 1 of flags_560 and updates with func_088160D8. */
void func_0881608C(Actor9C7248 *self)
{
    self->flags_560 = self->flags_560 & ~2u;
    func_088160D8(self);
}
