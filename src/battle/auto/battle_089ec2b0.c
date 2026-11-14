#include "battle/auto_region1.h"

/* True when active and in state 6. */
unsigned char func_089EC2B0(PartECED0 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->state == 6;
    }
}
