#include "main/auto_region1.h"

/* Deleting destructor of Obj9C6CA8. */
void func_0880B138(Obj9C6CA8 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C6CA8;
        func_08807788(self, 0);
        if ((flags & 1) != 0) {
            func_08807A1C(self);
        }
    }
}
