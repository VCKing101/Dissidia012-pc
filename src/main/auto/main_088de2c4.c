#include "main/auto_region4.h"

/* Obj9CC290: deleting destructor. */
void func_088DE2C4(Obj9CC290 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089CC290;
        func_088DE378(self);
        func_088DF4D8(&self->field_80, 2);
        func_0899372C(self->field_48, 2, sizeof(Obj9CC2D0), func_088DECB0, 0, 0);
        func_088DE968(&self->field_30, 2);
        func_088DF084(&self->field_14, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
