#include "main/auto_region1.h"

/* Resets the Obj9C74E8 and marks it active; returns 0. */
int func_08819C60(Obj9C74E8 *self)
{
    func_08819C44(self);
    self->active = 1;
    return 0;
}
