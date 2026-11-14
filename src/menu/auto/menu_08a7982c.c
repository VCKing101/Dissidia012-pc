#include "menu/auto_region3.h"

/* Step: queues state 0 on value 9. */
void func_08A7982C(Step *self, StepOwner *owner, int value)
{
    if (value == 9) {
        func_08A79E68(&owner->queue, 0);
    }
}
