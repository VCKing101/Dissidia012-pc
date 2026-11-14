#include "main/auto_region2.h"

/* Releases the object and marks it active; returns 0. */
int func_0887CFAC(Obj9C9D48 *self)
{
    func_0887CF80(self);
    self->active = 1;
    return 0;
}
