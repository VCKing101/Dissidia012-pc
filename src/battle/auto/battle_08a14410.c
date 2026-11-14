#include "battle/auto_region2.h"

/* Sets field_08 to 2 when func_08A23054 reports 0. */
int func_08A14410(Unit14458 *self)
{
    if (func_08A23054(self->field_1F4, self->field_1F8, 0xE) == 0) {
        self->field_08 = 2;
    }
    return 0;
}
