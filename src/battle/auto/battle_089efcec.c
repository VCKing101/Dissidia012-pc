#include "battle/auto_region1.h"

/* UnitF0C68 deleting destructor. */
void func_089EFCEC(UnitF0C68 *self, int flags)
{
    if (self) {
        self->vtable = D_089F0C68;
        func_089EFDB8(self);
        func_08A081B8(self->field_114, 2);
        func_08A0E3D8(self->field_7C, 2);
        func_089F8248(&self->field_44, 2);
        func_089EF2C8(&self->field_1C, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
