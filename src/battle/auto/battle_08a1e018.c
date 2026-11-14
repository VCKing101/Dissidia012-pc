#include "battle/auto_region3.h"

/* Sets field_08 to -1 once field_CC (a Part01DA8) has finished. */
int func_08A1E018(Unit1E058 *self)
{
    if (func_08A01A9C(&self->field_CC) == 1) {
        self->field_08 = -1;
    }
    return 0;
}
