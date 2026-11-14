#include "battle/auto_region5.h"

/* Part488E8: sets field_60, and field_3C unless arg2 is -1. */
void func_08B48A38(Part488E8 *self, int arg1, int arg2)
{
    if (arg2 != -1) {
        self->field_3C = arg2;
    }
    self->field_60 = arg1;
}
