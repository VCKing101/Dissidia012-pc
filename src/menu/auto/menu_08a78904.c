#include "menu/auto_region3.h"

/* Step: tells owner->field_238 (5) and queues state 0xF. */
void func_08A78904(Step *self, StepOwner *owner)
{
    func_08A6F170(owner->field_238, 5, 0);
    func_08A79E68(&owner->queue, 0xF);
}
