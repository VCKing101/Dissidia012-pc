#include "battle/auto_region4.h"

/* PartD9580 deleting destructor: frees the three handles, then destroys
 * the base. */
void func_08AD7888(PartD9580 *self, int flags)
{
    if (self) {
        self->vtable = D_08AD9580;
        if (self->field_124 != 0) {
            func_088A6C70(self->field_124);
            self->field_124 = 0;
        }
        if (self->field_128 != 0) {
            func_088A6C70(self->field_128);
            self->field_128 = 0;
        }
        if (self->field_12C != 0) {
            func_088A6C70(self->field_12C);
            self->field_12C = 0;
        }
        func_08AD7E48(self);
        func_08AD2A10(self, 0);
        if (flags & 1) {
            func_08AD2F38(self);
        }
    }
}
