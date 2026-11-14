#include "battle/auto_region5.h"

/* Releases the handle at field_0C and runs func_088542AC on D_089D4DC8. */
void func_08B52DB8(Part4EE2C *self)
{
    func_0899BB1C(self->field_0C);
    self->field_0C = 0;
    func_088542AC(D_089D4DC8);
}
