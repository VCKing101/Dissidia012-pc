#include "battle/auto_region1.h"

/* True when active and in state 5. */
unsigned char func_089EC288(PartECED0 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->state == 5;
    }
}
