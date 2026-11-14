#include "main/auto_region2.h"

/* Deleting destructor: destroys the member at +0x288. */
void func_08879CB4(Obj879CB4 *self, unsigned int flags)
{
    if (self != 0) {
        func_0887A644(&self->field_288, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
