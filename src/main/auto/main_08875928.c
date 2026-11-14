#include "main/auto_region2.h"

/* Releases the object and marks it active; returns 0. */
int func_08875928(Obj9C9C38 *self)
{
    func_0887590C(self);
    self->active = 1;
    return 0;
}
