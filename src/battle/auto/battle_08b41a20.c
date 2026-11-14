#include "battle/auto_region5.h"

/* Handle41620: sets the handle's value, if any. */
void func_08B41A20(Handle41620 *self, const void *value)
{
    if (self->handle != 0) {
        func_088D9AF4(self->handle, value);
    }
}
