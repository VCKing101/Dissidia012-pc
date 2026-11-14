#include "battle/auto_region3.h"

/* Sets field_04 to -1 once field_4C (a Part26140) has finished. */
int func_08A25A98(Part25AD8 *self)
{
    if (func_08A25F3C(&self->field_4C) == 1) {
        self->field_04 = -1;
    }
    return 0;
}
