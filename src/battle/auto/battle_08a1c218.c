#include "battle/auto_region3.h"

/* Once field_88 (a Part21C08) has finished, calls func_08A01C74(field_18, 5)
 * and sets field_08 to 6. */
int func_08A1C218(Unit1C398 *self)
{
    if (func_08A219D8(&self->field_88) == 1) {
        func_08A01C74(&self->field_18, 5);
        self->field_08 = 6;
    }
    return 0;
}
