#include "battle/auto_region3.h"

/* Sets field_04 to -1 once field_D8 (a Part08388) has finished. */
int func_08A17C50(Part17C90 *self)
{
    if (func_08A0836C(&self->field_D8) == 1) {
        self->field_04 = -1;
    }
    return 0;
}
