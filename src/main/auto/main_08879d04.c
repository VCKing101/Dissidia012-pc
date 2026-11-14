#include "main/auto_region2.h"

/* Deleting destructor: destroys the member at +0x14. */
void func_08879D04(Obj879D04 *self, unsigned int flags)
{
    if (self != 0) {
        func_0889F674(self->field_14, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
