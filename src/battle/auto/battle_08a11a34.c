#include "battle/auto_region2.h"

/* True when active and field_04 is not set. */
unsigned char func_08A11A34(Part11A58 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_04 == 0;
    }
}
