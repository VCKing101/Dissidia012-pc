#include "battle/auto_region3.h"

/* True when active and field_04 is -1. */
unsigned char func_08A179D8(Part17C90 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_04 == -1;
    }
}
