#include "battle/auto_region3.h"

/* Part64E48 deleting destructor. */
void func_08A63868(Part64E48 *self, int flags)
{
    if (self) {
        self->vtable = D_08A64E48;
        func_08A6396C(self);
        func_08AA2A30(self->field_13A0, 2);
        func_08A9A1F0(self->field_138C, 2);
        func_08AA1F10(self->field_135C, 2);
        func_08A9A578(self->field_1330, 2);
        func_08A9AC58(self->field_11C0, 2);
        func_08A65EA8(&self->field_FD0, 2);
        func_08A65710(&self->field_C9C, 2);
        func_08AA4CE8(self->field_C80, 2);
        func_08A807A8(&self->field_C10, 2);
        func_08A871B8(self->field_5B0, 2);
        func_08A963E0(self->field_530, 2);
        func_08AB2F60(self->field_20, 2);
        if (flags & 1) {
            func_08A63A10(self);
        }
    }
}
