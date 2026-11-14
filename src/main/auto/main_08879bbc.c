#include "main/auto_region2.h"

/* Deleting destructor: destroys the element at +0x148. */
void func_08879BBC(Obj879BBC *self, unsigned int flags)
{
    if (self != 0) {
        func_0899372C(self->field_148, 1, 0x14, func_0892E670, 0, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
