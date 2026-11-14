#include "battle/auto_region3.h"

/* Once field_138 (a Part08388) has finished, calls func_08A01C74(field_1C,
 * 5) and sets field_08 to 10. */
int func_08A1B358(Unit1B3F8 *self)
{
    if (func_08A0836C(&self->field_138) == 1) {
        func_08A01C74(&self->field_1C, 5);
        self->field_08 = 10;
    }
    return 0;
}
