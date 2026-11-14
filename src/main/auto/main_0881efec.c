#include "main/auto_region2.h"

/* Deleting destructor of Obj9C7638. */
void func_0881EFEC(Obj9C7638 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C7638;
        func_0881F068(self);
        func_088201E8(&self->field_24, 2);
        func_0881F424(&self->field_08, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
