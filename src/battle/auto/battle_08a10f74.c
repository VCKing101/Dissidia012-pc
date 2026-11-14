#include "battle/auto_region2.h"

/* True when active and field_04 is not set. */
unsigned char func_08A10F74(Part10F98 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_04 == 0;
    }
}
