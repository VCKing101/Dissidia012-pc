#include "battle/auto_region3.h"

/* Once field_138 (a Part08388) has finished, calls func_08A01C74(field_18,
 * 5) and sets field_08 to 10. */
int func_08A1C2FC(Unit1C398 *self)
{
    if (func_08A0836C(&self->field_138) == 1) {
        func_08A01C74(&self->field_18, 5);
        self->field_08 = 10;
    }
    return 0;
}
