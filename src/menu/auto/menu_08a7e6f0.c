#include "menu/auto_region3.h"

/* Step77C34: once both motions end, sets owner->field_274 and queues state 0xC. */
void func_08A7E6F0(Step77C34 *self, StepOwner *owner)
{
    int done;

    done = func_08A74564(&self->field_04);
    if ((done == 1) && (done = func_08A7304C(&self->field_14), done == 1)) {
        owner->field_274 = 1;
        func_08A79E68(&owner->queue, 0xC);
    }
}
