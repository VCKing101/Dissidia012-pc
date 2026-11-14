#include "battle/auto_region4.h"

/* PartD9580: steps, then sets field_104 to 1 once the handle at field_124
 * reports 0. */
void func_08AD7BF4(PartD9580 *self)
{
    func_08AD8DA8(self);
    func_08AD8284(self, 1);
    if (self->field_124 != 0 && func_088A751C(self->field_124) == 0) {
        self->field_104 = 1;
    }
}
