#include "battle/auto_region1.h"

/* True when active and field_04 is -1. */
unsigned char func_089EF7E4(PartEFC58 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_04 == -1;
    }
}
