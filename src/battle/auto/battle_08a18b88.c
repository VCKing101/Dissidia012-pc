#include "battle/auto_region3.h"

/* Sets field_08 to 10 once field_14 (a Part01DA8) has finished. */
int func_08A18B88(Unit18BD8 *self)
{
    if (func_08A01A9C(&self->field_14) == 1) {
        self->field_08 = 10;
    }
    return 0;
}
