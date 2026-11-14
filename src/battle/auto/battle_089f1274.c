#include "battle/auto_region1.h"

/* True when active and field_08 is 0. */
unsigned char func_089F1274(PartF12D8 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_08 == 0;
    }
}
