#include "battle/auto_region3.h"

/* Unit279D0 deleting destructor. */
void func_08A27304(Unit279D0 *self, int flags)
{
    if (self) {
        self->vtable = D_08A279D0;
        func_08A273A0(self);
        func_08A081B8(&self->field_14, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
