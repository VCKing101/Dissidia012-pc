#include "battle/auto_region1.h"

/* True when active and field_0C is 1. */
unsigned char func_089F8644(PartF8670 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_0C == 1;
    }
}
