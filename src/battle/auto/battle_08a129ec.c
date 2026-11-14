#include "battle/auto_region5.h"

/* Unit12A88 step: sets field_08 to 0xC once field_C4 and field_280 have
 * finished. */
int func_08A129EC(Unit12A88 *self)
{
    if (func_08A10F74(&self->field_C4) == 1 && func_08A0836C(&self->field_280) == 1) {
        self->field_08 = 0xC;
    }
    return 0;
}
