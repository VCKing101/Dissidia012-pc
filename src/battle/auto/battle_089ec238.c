#include "battle/auto_region1.h"

/* True when active and in state 3. */
unsigned char func_089EC238(PartECED0 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->state == 3;
    }
}
