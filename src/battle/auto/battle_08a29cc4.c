#include "battle/auto_region3.h"

/* Sets field_08 to -1 once field_1C (a Part01DA8) has finished. */
int func_08A29CC4(Unit29D08 *self)
{
    if (func_08A01A9C(&self->field_1C) == 1) {
        self->field_08 = -1;
    }
    return 0;
}
