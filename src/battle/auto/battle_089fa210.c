#include "battle/auto_region1.h"

/* Sets field_08 to -1 when func_08A01A9C returns 1 for field_14. */
int func_089FA210(UnitFA2C8 *self)
{
    if (func_08A01A9C(self->field_14) == 1) {
        self->field_08 = -1;
    }
    return 0;
}
