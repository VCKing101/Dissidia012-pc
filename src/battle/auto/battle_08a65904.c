#include "battle/auto_region3.h"

/* True when active and field_324 is not set. */
unsigned char func_08A65904(Part65E48 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_324 == 0;
    }
}
