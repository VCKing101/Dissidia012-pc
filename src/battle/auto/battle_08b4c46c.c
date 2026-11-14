#include "battle/auto_region5.h"

/* Handle4C22C: hides the handle, if any. */
void func_08B4C46C(Handle4C22C *self)
{
    if (self->handle != 0) {
        func_088D9CFC(self->handle);
    }
}
