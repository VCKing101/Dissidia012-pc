#include "main/auto_region2.h"

/* Deleting destructor: destroys the members at +0xD0 and +0x10. */
void func_08879F28(Obj879F28 *self, unsigned int flags)
{
    if (self != 0) {
        func_088734D4(self->field_D0, 2);
        func_0885E834(self->field_10, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
