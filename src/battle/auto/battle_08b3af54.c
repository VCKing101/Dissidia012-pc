#include "battle/auto_region5.h"

/* Part3AF54: clears field_119, runs field_1E0, and unless field_118 is set
 * runs func_08B3B3EC with field_110. */
void func_08B3AF54(Part3AF54 *self)
{
    self->field_119 = 0;
    func_0880FE20(self->field_1E0);
    if (self->field_118 == 0) {
        func_08B3B3EC(self, self->field_110, 0);
    }
}
