#include "battle/auto_region2.h"

/* True when active and field_08 is not set. */
unsigned char func_08A01A9C(void *object)
{
    Part01DA8 *self = object;

    if (!self->active) {
        return 0;
    } else {
        return self->field_08 == 0;
    }
}
