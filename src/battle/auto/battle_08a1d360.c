#include "battle/auto_region3.h"

/* Once field_C4 (a Part10F98) has finished, calls func_08A01C74(field_14,
 * 4), func_08A01CF4(field_64) and sets field_08 to 9. */
int func_08A1D360(Unit1D3F8 *self)
{
    if (func_08A10F74(&self->field_C4) == 1) {
        func_08A01C74(&self->field_14, 4);
        func_08A01CF4(&self->field_64);
        self->field_08 = 9;
    }
    return 0;
}
