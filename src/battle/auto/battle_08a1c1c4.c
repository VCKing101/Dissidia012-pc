#include "battle/auto_region3.h"

/* Once field_64 (a Part02198) has finished, calls func_08A219FC(field_88,
 * field_14), func_089F854C(field_100) and sets field_08 to 5. */
int func_08A1C1C4(Unit1C398 *self)
{
    if (func_08A020A4(&self->field_64) == 1) {
        func_08A219FC(&self->field_88, self->field_14);
        func_089F854C(&self->field_100);
        self->field_08 = 5;
    }
    return 0;
}
