#include "battle/auto_region5.h"

/* Part06984: goes to state 10 once field_2C reaches 60, and counts it up by
 * the frame time. */
int func_08B06D8C(Part06984 *self)
{
    if (60.0f <= self->field_2C) {
        func_08B081D4(self, 10);
    }
    self->field_2C = self->field_2C + D_089D69E0->field_18;
    return 0;
}
