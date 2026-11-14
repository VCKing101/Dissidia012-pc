#include "menu/auto_region2.h"

/* Item: sets field_F8, then func_08A5585C and func_08A558D8. */
void func_08A550A0(Item *self)
{
    self->field_F8 = 1;
    func_08A5585C(self);
    func_08A558D8(self);
}
