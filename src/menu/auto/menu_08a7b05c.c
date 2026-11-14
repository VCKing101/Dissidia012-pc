#include "menu/auto_region3.h"

/* Step7ADD8: once the motion ends, sets owner->field_274 and queues state 0xC. */
void func_08A7B05C(Step7ADD8 *self, StepOwner *owner)
{
    int done;

    done = func_08A772AC(&self->field_04);
    if (done == 1) {
        owner->field_274 = 1;
        func_08A79E68(&owner->queue, 0xC);
    }
}
