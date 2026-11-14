#include "menu/auto_region3.h"

/* Step7D0A0: queues state 8 or 9 by field_21C. */
void func_08A7D0A0(Step7D0A0 *self, StepOwner *owner)
{
    if (self->field_21C == 0) {
        func_08A79E68(&owner->queue, 8);
    }
    else {
        func_08A79E68(&owner->queue, 9);
    }
}
