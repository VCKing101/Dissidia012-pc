#include "battle/auto_region3.h"

/* Part15B58 release: calls func_08A15AF8 while field_01 is 1, then resets. */
void func_08A15A04(Part15B58 *self)
{
    if (self->field_01 == 1) {
        func_08A15AF8(self);
    }
    func_08A159F8(self);
}
