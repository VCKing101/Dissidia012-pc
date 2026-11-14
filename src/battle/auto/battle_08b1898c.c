#include "battle/auto_region5.h"

/* Obj18FF0 constructor. */
Obj18FF0 *func_08B1898C(Obj18FF0 *self, int value)
{
    self->vtable = D_08B18FF0;
    self->field_04 = value;
    return self;
}
