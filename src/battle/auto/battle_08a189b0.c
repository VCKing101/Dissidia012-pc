#include "battle/auto_region3.h"

/* When field_0C is 1, calls func_08A16B78(field_84) and sets field_08 to 5. */
int func_08A189B0(Unit18BD8 *self)
{
    if (self->field_0C == 1) {
        func_08A16B78(&self->field_84);
        self->field_08 = 5;
    }
    return 0;
}
