#include "battle/auto_region2.h"

/* True when active and field_08 is not set. */
unsigned char func_08A11270(Part11298 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_08 == 0;
    }
}
