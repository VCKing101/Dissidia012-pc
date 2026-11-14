#include "battle/auto_region1.h"

/* True when active and field_01 is clear. */
unsigned char func_089F8184(PartF81A8 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_01 == 0;
    }
}
