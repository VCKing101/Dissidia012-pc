#include "battle/auto_region5.h"

/* Part3E840: sets field_04, or 0 when `value` is not below D_089D4790's
 * field_08. */
void func_08B3E840(Part3E840 *self, int value)
{
    if (D_089D4790->field_08 <= value) {
        value = 0;
    }
    self->field_04 = value;
}
