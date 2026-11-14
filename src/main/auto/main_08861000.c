#include "main/auto_region2.h"

/* Releases the object and marks it active; returns 0. */
int func_08861000(Obj9C8E60 *self)
{
    func_08860FE4(self);
    self->active = 1;
    return 0;
}
