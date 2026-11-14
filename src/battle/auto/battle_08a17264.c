#include "battle/auto_region3.h"

/* True when active and field_04 is not set. */
unsigned char func_08A17264(Part174B8 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_04 == 0;
    }
}
