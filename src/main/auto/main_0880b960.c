#include "main/auto_region1.h"

/* Deleting destructor of Obj9C6D18: also clears the word field_208 points at. */
void func_0880B960(Obj9C6D18 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C6D18;
        *self->field_208 = 0;
        func_08807788(self, 0);
        if ((flags & 1) != 0) {
            func_08807A1C(self);
        }
    }
}
