#include "battle/auto_region3.h"

/* Once field_60 (a Part02198) has finished, calls func_08A08350(field_114)
 * and sets field_08 to 8. */
int func_08A18AEC(Unit18BD8 *self)
{
    if (func_08A020A4(&self->field_60) == 1) {
        func_08A08350(&self->field_114);
        self->field_08 = 8;
    }
    return 0;
}
