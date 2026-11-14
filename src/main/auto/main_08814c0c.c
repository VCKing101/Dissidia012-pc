#include "main/auto_region1.h"

/* Deleting destructor of Actor9C7248. */
void func_08814C0C(Actor9C7248 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable_544 = &D_089C7248;
        func_08814D08(self);
        func_08821110(&self->field_5DC, 2);
        func_08817C7C(&self->field_5B0, 2);
        func_088222CC(&self->field_570, 2);
        func_088310BC(self, 0);
        if ((flags & 1) != 0) {
            func_0883133C(self);
        }
    }
}
