#include "battle/auto_region3.h"

/* Once the timer in field_F0 has expired, signals field_1C (mode 5) and sets
 * field_08 to 6. */
int func_08A1B250(Unit1B3F8 *self)
{
    if (func_089F6388(&self->field_F0) == 1) {
        func_08A01C74(&self->field_1C, 5);
        self->field_08 = 6;
    }
    return 0;
}
