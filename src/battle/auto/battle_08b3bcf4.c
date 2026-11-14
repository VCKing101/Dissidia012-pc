#include "battle/auto_region5.h"

/* Part3BBFC: destroys object `index` of field_6D0. */
void func_08B3BCF4(Part3BBFC *self, int index)
{
    func_08B3AD98(self->field_6D0[index], 3);
    self->field_6D0[index] = 0;
}
