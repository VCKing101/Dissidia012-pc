#include "main/auto_region4.h"

/* Obj9CC330: deleting destructor. */
void func_088DF4D8(Obj9CC330 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089CC330;
        func_088DF54C(self);
        func_0887A2EC(&self->field_0C, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
