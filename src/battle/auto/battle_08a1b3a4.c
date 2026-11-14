#include "battle/auto_region3.h"

/* Sets field_08 to 0xB once field_1C (a Part01DA8) has finished. */
int func_08A1B3A4(Unit1B3F8 *self)
{
    if (func_08A01A9C(&self->field_1C) == 1) {
        self->field_08 = 0xB;
    }
    return 0;
}
