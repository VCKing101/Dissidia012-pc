#include "menu/auto_region1.h"

/* Screen0B6A0: clears flags 0x1000E0. */
void func_089F5C1C(Screen0B6A0 *self)
{
    self->work->flags = self->work->flags & 0xFFEFFF1F;
}
