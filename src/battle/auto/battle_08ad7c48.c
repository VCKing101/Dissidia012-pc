#include "battle/auto_region4.h"

/* PartD9580: frees each handle that reports 1, then sets field_104 to 2. */
void func_08AD7C48(PartD9580 *self)
{
    func_08AD8284(self, 0);
    if (self->field_124 != 0 && func_088A751C(self->field_124) == 1) {
        func_088A6AA4(self->field_124);
        self->field_124 = 0;
    }
    if (self->field_128 != 0 && func_088A751C(self->field_128) == 1) {
        func_088A6AA4(self->field_128);
        self->field_128 = 0;
    }
    if (self->field_12C != 0 && func_088A751C(self->field_12C) == 1) {
        func_088A6AA4(self->field_12C);
        self->field_12C = 0;
    }
    self->field_104 = 2;
}
