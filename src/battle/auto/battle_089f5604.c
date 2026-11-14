#include "battle/auto_region1.h"

/* True when active and no resource is held at field_04. */
unsigned char func_089F5604(PartF5628 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_04 == 0;
    }
}
