#include "main/auto_region4.h"

/* Obj9CC308: deleting destructor. */
void func_088DF084(Obj9CC308 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089CC308;
        func_088DF100(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
