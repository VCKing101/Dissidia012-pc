#include "battle/auto_region2.h"

/* Sets field_08 to -1 once field_40 (a Part01DA8) has finished. */
int func_08A0B9D0(Unit0BAF0 *self)
{
    if (func_08A01A9C(&self->field_40) == 1) {
        self->field_08 = -1;
    }
    return 0;
}
