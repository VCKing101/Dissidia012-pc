#include "battle/auto_region1.h"

/* True when active and field_F4 is 0. */
unsigned char func_089F5C54(PartF5E58 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_F4 == 0;
    }
}
