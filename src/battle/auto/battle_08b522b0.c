#include "battle/auto_region5.h"

/* Releases the handle at field_34 and runs func_0885ACE0 on D_089D4DC8's
 * field_160. */
void func_08B522B0(Part522B0 *self)
{
    func_0899BB1C(self->field_34);
    self->field_34 = 0;
    func_0885ACE0(D_089D4DC8->field_160);
}
