#include "battle/auto_region1.h"

/* Sets field_08 to -1 when func_089F1FB8 returns 1 for field_14. */
int func_089F1ADC(UnitF1B20 *self)
{
    if (func_089F1FB8(&self->field_14) == 1) {
        self->field_08 = -1;
    }
    return 0;
}
