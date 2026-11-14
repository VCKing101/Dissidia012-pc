#include "battle/auto_region1.h"

/* From state 0 or 1: adds field_48 to the counter at field_40 and moves to
 * state 2; -1 when the counter is inactive. */
int func_089E8CA8(PartE8D08 *self)
{
    if (self->state == 0 || self->state == 1) {
        if (func_089DF854(self->field_40, self->field_48) != 0) {
            return -1;
        }
        self->state = 2;
    }
    return 0;
}
