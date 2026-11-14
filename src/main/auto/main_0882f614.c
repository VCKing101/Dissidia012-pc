#include "main/auto_region2.h"

/* Deleting destructor: destroys the part at +0x34. */
void func_0882F614(Obj82F614 *self, unsigned int flags)
{
    if (self != 0) {
        func_0886F048(self->field_34, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
