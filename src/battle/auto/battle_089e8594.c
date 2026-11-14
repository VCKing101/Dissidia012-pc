#include "battle/auto_region1.h"

/* True when active and in state -1. */
unsigned char func_089E8594(PartE8D08 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->state == -1;
    }
}
