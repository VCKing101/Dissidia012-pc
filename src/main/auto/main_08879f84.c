#include "main/auto_region2.h"

/* Deleting destructor: destroys the members at +0x100 and +0x40. */
void func_08879F84(Obj879F84 *self, unsigned int flags)
{
    if (self != 0) {
        func_088734D4(self->field_100, 2);
        func_0885E834(self->field_40, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
