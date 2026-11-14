#include "battle/auto_region3.h"

/* Part22AA0 deleting destructor. */
void func_08A21F90(Part22AA0 *self, int flags)
{
    if (self) {
        self->vtable = D_08A22AA0;
        func_08A2200C(self);
        func_08A06C18(&self->field_100, 2);
        func_08A23220(&self->field_04, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
