#include "battle/auto_region5.h"

/* Part61C08: sets field_04 and clears field_14. */
void func_08B61C08(Part61C08 *self, int value)
{
    self->field_14 = 0;
    self->field_04 = value;
}
