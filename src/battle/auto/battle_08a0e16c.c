#include "battle/auto_region2.h"

/* Sets field_08 to -1 once field_18 (a Part01DA8) has finished. */
int func_08A0E16C(Unit0E2E0 *self)
{
    if (func_08A01A9C(&self->field_18) == 1) {
        self->field_08 = -1;
    }
    return 0;
}
