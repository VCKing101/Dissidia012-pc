#include "battle/auto_region3.h"

/* Sets field_04 to -1 once field_DC (a Part08388) has finished. */
int func_08A26888(Part268C8 *self)
{
    if (func_08A0836C(&self->field_DC) == 1) {
        self->field_04 = -1;
    }
    return 0;
}
