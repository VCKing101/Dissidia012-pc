#include "battle/auto_region2.h"

/* Sets field_08 to 2 when func_08A23054 reports 0. */
int func_08A12A44(Unit12A88 *self)
{
    if (func_08A23054(self->field_2B4, self->field_2B8, 0xD) == 0) {
        self->field_08 = 2;
    }
    return 0;
}
