#include "battle/auto_region5.h"

/* Part06984: finishes field_60, registers func_08B0944C as a callback with
 * D_089D727C and sets field_98. */
void func_08B0817C(Part06984 *self)
{
    func_08B17270(&self->field_60);
    func_088B1354(D_089D727C, 0, func_08B0944C, self, 0xA0);
    self->field_98 = 0x10;
}
