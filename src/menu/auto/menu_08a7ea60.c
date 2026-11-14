#include "menu/auto_region3.h"

/* StepWait: queues state 0 once done. */
void func_08A7EA60(StepWait *self, StepOwner *owner)
{
    if (self->done == 1) {
        func_08A79E68(&owner->queue, 0);
    }
}
