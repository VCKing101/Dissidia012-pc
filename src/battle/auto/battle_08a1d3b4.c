#include "battle/auto_region3.h"

/* Sets field_08 to -1 once field_14 (a Part01DA8) has finished. */
int func_08A1D3B4(Unit1D3F8 *self)
{
    if (func_08A01A9C(&self->field_14) == 1) {
        self->field_08 = -1;
    }
    return 0;
}
