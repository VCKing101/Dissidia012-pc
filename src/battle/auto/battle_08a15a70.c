#include "battle/auto_region3.h"

/* True when active and field_01 is not set. */
unsigned char func_08A15A70(Part15B58 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_01 == 0;
    }
}
