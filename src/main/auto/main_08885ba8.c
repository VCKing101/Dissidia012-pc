#include "main/auto_region2.h"

/* Deleting destructor of Obj885B78. */
void func_08885BA8(Obj885B78 *self, unsigned int flags)
{
    if (self != 0) {
        func_0887A644(&self->field_10, 2);
        func_0887A644(&self->field_00, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
