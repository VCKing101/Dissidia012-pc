#include "battle/auto_region3.h"

/* Once field_74 (a Part02198) has finished, calls func_08A08350(field_144)
 * and sets field_08 to 10. */
int func_08A1537C(Unit15448 *self)
{
    if (func_08A020A4(&self->field_74) == 1) {
        func_08A08350(&self->field_144);
        self->field_08 = 10;
    }
    return 0;
}
