#include "battle/auto_region4.h"

/* PartAAD68 deleting destructor. */
void func_08AA77D0(PartAAD68 *self, int flags)
{
    if (self) {
        self->vtable = D_08AAAD68;
        func_08AA7850(self);
        func_08AA69DC(&self->field_DC, 2);
        func_08AA69DC(&self->field_08, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
