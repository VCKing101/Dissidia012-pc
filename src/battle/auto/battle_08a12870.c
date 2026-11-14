#include "battle/auto_region2.h"

/* Sets field_08 to 8 once field_98, field_C4 and field_130 have all finished. */
int func_08A12870(Unit12A88 *self)
{
    if (func_08A10F74(&self->field_98) == 1 && func_08A10F74(&self->field_C4) == 1
        && func_08A11A34(&self->field_130) == 1) {
        self->field_08 = 8;
    }
    return 0;
}
