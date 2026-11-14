#include "battle/auto_region5.h"

/* Handle4C22C: gives the handle the value, if any. */
void func_08B4C4C8(Handle4C22C *self, const void *value)
{
    if (self->handle != 0) {
        func_088D9AF4(self->handle, value);
    }
}
