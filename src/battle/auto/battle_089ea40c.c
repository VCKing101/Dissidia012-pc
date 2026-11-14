#include "battle/auto_region1.h"

/* Shuts the block down when func_0887AFB0 reports 1. */
void func_089EA40C(PartEA518 *self)
{
    if (func_0887AFB0(self->block) == 1) {
        func_0887AD1C(self->block);
    }
}
