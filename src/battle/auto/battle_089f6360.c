#include "battle/auto_region1.h"

/* True when active and field_30 is 1. */
unsigned char func_089F6360(PartF63E0 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_30 == 1;
    }
}
