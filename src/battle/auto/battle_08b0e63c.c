#include "battle/auto_region5.h"

/* Part0E63C deleting destructor: frees the four buffers and destroys
 * field_10. */
void func_08B0E63C(Part0E63C *self, int flags)
{
    if (self != 0) {
        if (self->field_00 != 0) {
            func_08890970(self->field_00);
        }
        if (self->field_04 != 0) {
            func_08890970(self->field_04);
        }
        if (self->field_08 != 0) {
            func_08890970(self->field_08);
        }
        if (self->field_0C != 0) {
            func_08890970(self->field_0C);
        }
        func_08B10A6C(self->field_10, 3);
        if ((flags & 1) && self != 0) {
            func_08890970(self);
        }
    }
}
