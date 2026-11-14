#include "battle/auto_region3.h"

/* Once field_64 (a Part02198) has finished, calls func_08A08350(field_138)
 * and sets field_08 to 9. */
int func_08A1C2B4(Unit1C398 *self)
{
    if (func_08A020A4(&self->field_64) == 1) {
        func_08A08350(&self->field_138);
        self->field_08 = 9;
    }
    return 0;
}
