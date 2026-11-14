#include "battle/auto_region3.h"

/* Part7ABD0 deleting destructor. */
void func_08A65EA8(Part7ABD0 *self, int flags)
{
    if (self) {
        self->vtable = D_08A7ABD0;
        func_08A65F70(self);
        func_08A94260(self->field_24, 2);
        func_08A7E778(&self->field_14, 2);
        func_08A7E778(&self->field_04, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
