#include "battle/auto_region3.h"

/* True when active and field_04 is not set. */
unsigned char func_08A25194(Part254D0 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_04 == 0;
    }
}
