#include "battle/auto_region1.h"

/* True when active and field_04 is 0. */
unsigned char func_089F1FB8(PartF1FE0 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_04 == 0;
    }
}
