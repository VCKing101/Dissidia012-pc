#include "battle/auto_region5.h"

/* Handle41620: runs func_088D9C60 on the handle, if any. */
void func_08B418A0(Handle41620 *self, int arg1, int arg2)
{
    if (self->handle != 0) {
        func_088D9C60(self->handle, arg1, arg2);
    }
}
