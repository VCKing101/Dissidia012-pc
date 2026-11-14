#include "battle/auto_region5.h"

/* Part05B88 release: runs func_08B05370, releases field_08 and resets. */
void func_08B0532C(Part05B88 *self)
{
    func_08B05370(self);
    if (self->field_08 != 0) {
        func_088DA060(self->field_08);
        self->field_08 = 0;
    }
    func_08B052F0(self);
}
