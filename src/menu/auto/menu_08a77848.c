#include "menu/auto_region3.h"

/* Step: clears owner->field_274 and queues state 0xC. */
void func_08A77848(Step *self, StepOwner *owner)
{
    owner->field_274 = 0;
    func_08A79E68(&owner->queue, 0xC);
}
