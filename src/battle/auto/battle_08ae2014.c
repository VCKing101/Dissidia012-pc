#include "battle/auto_region4.h"

/* PartE7B50: stores *value in field_29C when func_08AE37B0 reports 1 for
 * id 0x157. */
void func_08AE2014(PartE7B50 *self, int arg, int *value)
{
    if (func_08AE37B0(self, 0x157) == 1) {
        self->field_29C = *value;
    }
}
