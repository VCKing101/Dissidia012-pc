#include "main/auto_region1.h"

/* Sets bit 1 of flags_560 and updates with func_088160D8. */
void func_088160B4(Actor9C7248 *self)
{
    self->flags_560 = self->flags_560 | 2;
    func_088160D8(self);
}
