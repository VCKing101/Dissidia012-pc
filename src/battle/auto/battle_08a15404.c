#include "battle/auto_region3.h"

/* Sets field_08 to 2 when func_08A23054 reports 0. */
int func_08A15404(Unit15448 *self)
{
    if (func_08A23054(self->field_178, self->field_17C, 0xD) == 0) {
        self->field_08 = 2;
    }
    return 0;
}
