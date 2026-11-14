#include "main/auto_region2.h"

/* Deleting destructor of Obj9C9D30. */
void func_0887A2EC(Obj9C9D30 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C9D30;
        func_0887A354(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
