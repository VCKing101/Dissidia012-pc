#include "battle/auto_region5.h"

/* Unit14458 step: sets field_08 to 0xC once field_1C0 has finished. */
int func_08A143D0(Unit14458 *self)
{
    if (func_08A0836C(&self->field_1C0) == 1) {
        self->field_08 = 0xC;
    }
    return 0;
}
