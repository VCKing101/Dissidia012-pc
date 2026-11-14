#include "battle/auto_region3.h"

/* Once field_114 (a Part08388) has finished, calls func_08A01C74(field_14,
 * 9) and sets field_08 to 9. */
int func_08A18B34(Unit18BD8 *self)
{
    if (func_08A0836C(&self->field_114) == 1) {
        func_08A01C74(&self->field_14, 9);
        self->field_08 = 9;
    }
    return 0;
}
