#include "battle/auto_region3.h"

/* When field_0D is 1, calls func_08A020C8(field_74) and sets field_08 to 9. */
int func_08A1533C(Unit15448 *self)
{
    if (self->field_0D == 1) {
        func_08A020C8(&self->field_74);
        self->field_08 = 9;
    }
    return 0;
}
