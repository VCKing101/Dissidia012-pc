#include "main/auto_region2.h"

/* Deleting destructor: destroys the members at +0x1C and +0x0C. */
void func_08879D54(Obj879D54 *self, unsigned int flags)
{
    if (self != 0) {
        func_0889F674(self->field_1C, 2);
        func_0887A644(&self->field_0C, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
