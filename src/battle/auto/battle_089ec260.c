#include "battle/auto_region1.h"

/* True when active and in state 4. */
unsigned char func_089EC260(PartECED0 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->state == 4;
    }
}
