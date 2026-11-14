#include "battle/auto_region5.h"

/* Handle41620: shows the handle and plays motion 1 (phase 1). */
void func_08B41790(Handle41620 *self)
{
    if (self->phase == 0 && self->handle != 0) {
        func_088D9CD8(self->handle);
        func_088D9F5C(self->handle, 1, 1);
        self->phase = 1;
    }
}
