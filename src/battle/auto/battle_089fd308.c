#include "battle/auto_region2.h"

/* Sets field_08 to -1 once field_14 (a Part01DA8) has finished. */
int func_089FD308(UnitFD348 *self)
{
    if (func_08A01A9C(&self->field_14) == 1) {
        self->field_08 = -1;
    }
    return 0;
}
