#include "battle/auto_region5.h"

/* PartFF60C: stores `value` in field_08 and field_0C and reports the change
 * through func_08AE3B54. */
void func_08AFF60C(PartFF60C *self, int value)
{
    int previous;

    previous = self->field_08;
    self->field_08 = value;
    func_08AE3B54(0.0f, 8, value, value - previous, 0);
    self->field_0C = value;
}
