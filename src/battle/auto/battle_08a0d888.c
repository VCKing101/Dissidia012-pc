#include "battle/auto_region2.h"

/* True when active and field_04 is not set. */
unsigned char func_08A0D888(Part0D948 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_04 == 0;
    }
}
