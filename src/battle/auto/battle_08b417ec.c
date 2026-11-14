#include "battle/auto_region5.h"

/* Handle41620: in phase 2, plays motion 0 (phase 3). */
void func_08B417EC(Handle41620 *self)
{
    if (self->phase == 2) {
        func_088D9F5C(self->handle, 0, 1);
        self->phase = 3;
    }
}
