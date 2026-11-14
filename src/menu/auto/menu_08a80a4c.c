#include "menu/auto_region3.h"

/* Screen7EEDC: 1 for kind 0x17 unless the mode is 1 or 3. */
int func_08A80A4C(Screen7EEDC *self, int kind)
{
    int result;

    result = 0;
    if (kind == 0x17) {
        result = 1;
        if ((self->work->mode == 1) || (self->work->mode == 3)) {
            result = 0;
        }
    }
    return result;
}
