#include "battle/auto_region2.h"

/* Part11A58 deleting destructor. */
void func_08A116A0(Part11A58 *self, int flags)
{
    if (self) {
        self->vtable = D_08A11A58;
        func_08A11724(self);
        func_089F7C80(&self->field_08, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
