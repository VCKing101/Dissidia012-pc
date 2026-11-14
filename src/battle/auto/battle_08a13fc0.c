#include "battle/auto_region2.h"

/* Sets field_08 to -1 once field_24 (a Part01DA8) has finished. */
int func_08A13FC0(Unit14458 *self)
{
    if (func_08A01A9C(&self->field_24) == 1) {
        self->field_08 = -1;
    }
    return 0;
}
