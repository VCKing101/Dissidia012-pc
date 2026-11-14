#include "menu/auto_region3.h"

/* StepWait: opens the owner's dialog; func_08A79B84 sets done when it closes. */
void func_08A79AD0(StepWait *self, StepOwner *owner)
{
    func_088B1194(D_089D727C, owner->field_260, func_08A79B84, (int)self, 0xA0);
    self->done = 0;
}
