#include "battle/auto_region5.h"

/* Part126DC: clears field_08's field_88C when func_0887066C reports 0. */
void func_08B126DC(Part126DC *self)
{
    if (func_0887066C(D_089D5FB4) == 0) {
        self->field_08->field_88C = 0;
    }
}
