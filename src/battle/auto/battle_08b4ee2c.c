#include "battle/auto_region5.h"

/* Releases the handle at field_0C and runs func_0885ACE0 on D_089D4DC8's
 * field_160. */
void func_08B4EE2C(Part4EE2C *self)
{
    func_0899BB1C(self->field_0C);
    self->field_0C = 0;
    func_0885ACE0(D_089D4DC8->field_160);
}
