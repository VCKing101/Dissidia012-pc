#include "menu/auto_region3.h"

/* ScreenA3F44 deleting destructor. */
void func_08A9CC28(ScreenA3F44 *self, unsigned int flags)
{
    if ((self != 0) && (func_08AB4228(&self->field_894, 2), (flags & 1) != 0)) {
        func_08992F04(self);
    }
}
