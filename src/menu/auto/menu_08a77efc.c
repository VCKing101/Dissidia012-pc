#include "menu/auto_region3.h"

/* Step77C34: once both motions end, queues state 0xF. */
void func_08A77EFC(Step77C34 *self, StepOwner *owner)
{
    int done;

    done = func_08A74564(&self->field_04);
    if ((done == 1) && (done = func_08A7304C(&self->field_14), done == 1)) {
        func_08A79E68(&owner->queue, 0xF);
    }
}
