#include "battle/auto_region3.h"

/* Sets field_08 to 7 once field_18 (a Part01DA8) has finished. */
int func_08A1C264(Unit1C398 *self)
{
    if (func_08A01A9C(&self->field_18) == 1) {
        self->field_08 = 7;
    }
    return 0;
}
