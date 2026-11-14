#include "main/auto_region2.h"

/* Releases the handle. */
void func_088542AC(Handle *self)
{
    func_0899BB1C(self->handle);
    self->handle = 0;
}
