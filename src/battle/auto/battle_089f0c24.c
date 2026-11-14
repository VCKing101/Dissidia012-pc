#include "battle/auto_region1.h"

/* Sets field_08 to -1 when func_089EF7E4 returns 1 for field_1C. */
int func_089F0C24(UnitF0C68 *self)
{
    if (func_089EF7E4(&self->field_1C) == 1) {
        self->field_08 = -1;
    }
    return 0;
}
