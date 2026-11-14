#include "battle/auto_region5.h"

/* Part114A0 deleting destructor: sets the byte field_00 points to. */
void func_08B114A0(Part114A0 *self, int flags)
{
    if (self != 0) {
        *self->field_00 = 1;
        if ((flags & 1) && self != 0) {
            func_08890970(self);
        }
    }
}
